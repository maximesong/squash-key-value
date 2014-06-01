#include <iostream>
#include <fstream>
#include <map>

#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>

#include "protocol.h"
#include "json11.hpp"

using namespace std;
using namespace json11;


const int SERVER_PORT = 9000;
const char *SERVER_HOST = "127.0.0.1";

const int RESPONSE_BUFF_SIZE = 4096;
const int VALUE_BUFF_SIZE = 4096;

int put(int sockfd, const char *key, int key_len,
	 const char* value, int value_len) {
	Head head = Head::makePut(key_len, value_len);
	char *buff = new char[head.bufferSize()];
	head.makePackage(buff, key, value);
	send(sockfd, buff, head.bufferSize(), 0);
	delete[] buff;

	char recv_buff[RESPONSE_BUFF_SIZE];
	int len = recv(sockfd, recv_buff, RESPONSE_BUFF_SIZE, 0);
	assert(len >= 0);
	Head recv_head = Head::from(recv_buff);
	assert(recv_head.getMethod() == Head::OK);
	return recv_head.getMethod() == Head::OK;
}

int get(int sockfd, const char *key, int key_len,
	 char *value) {
	Head head = Head::makeGet(key_len);
	char *buff = new char[head.bufferSize()];
	head.makePackage(buff, key);
	send(sockfd, buff, head.bufferSize(), 0);
	delete[] buff;

	char recv_buff[RESPONSE_BUFF_SIZE];
	int len = recv(sockfd, recv_buff, RESPONSE_BUFF_SIZE, 0);
	assert(len >= 0);
	Head recv_head = Head::from(recv_buff);
	recv_head.extract(value, recv_buff);
	return recv_head.getKeyLength();
}

void put_str(int sockfd, const char *key, const char *value) {
	put(sockfd, key, strlen(key) + 1, value, strlen(value) + 1);
}

int get_str(int sockfd, const char *key, char *value) {
	return get(sockfd, key, strlen(key) + 1, value);
}

int connectSocket() {
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(0);
	addr.sin_addr.s_addr = htons(INADDR_ANY);

	bzero(&(addr.sin_zero), 8);
	bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));

	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
	connect(sockfd, (struct sockaddr *)&server_addr, 
		sizeof(struct sockaddr));

	return sockfd;
}

void gen_random(char *s, const int len) {
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i) {
		s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
	}

	s[len] = 0;
}

void get_stats(int sockfd) {
	Head head = Head::makeStats();
	char *buff = new char[head.bufferSize()];
	head.makePackage(buff);
	send(sockfd, buff, head.bufferSize(), 0);
	delete[] buff;

	char recv_buff[RESPONSE_BUFF_SIZE];
	int len = recv(sockfd, recv_buff, RESPONSE_BUFF_SIZE, 0);
	assert(len >= 0);
	Head recv_head = Head::from(recv_buff);
	assert(recv_head.getMethod() == Head::OK);
}

void test_random() {
	char value_buff[VALUE_BUFF_SIZE];
	char keys[1024][128];
	char values[1024][1024];

	// if you need different random for various invokation, use srand
	//srand (time(NULL));

	for (int i = 0; i != 1024; ++i) {
		cout << i << endl;
		gen_random(keys[i], 128 - 1);
		gen_random(values[i], 1024 - 1);
		int sockfd = connectSocket();
		put_str(sockfd, keys[i], values[i]);
		close(sockfd);

		sockfd = connectSocket();
		get_str(sockfd, keys[i], value_buff);
		cout << value_buff << endl;
		close(sockfd);
	}
	int sockfd = connectSocket();
	get_stats(sockfd);
}

string read_file(const char *filename) {
  std::ifstream is(filename);
  if (is) {
    // get length of file:
    is.seekg (0, is.end);
    int length = is.tellg();
    is.seekg (0, is.beg);

    char * buffer = new char[length];
    is.read (buffer,length);

    is.close();
    string result{buffer};

    delete[] buffer;
    return result;
  }
  return "";
}

void test_top_sites() {
  string text = read_file("tools/sites.json");
  string error;
  Json parsed = Json::parse(text, error);
  assert(parsed.is_array());
  Json::array arr = parsed.array_items();
  for (Json j : arr) {
    assert(j.is_object());
    cout << j["site"].string_value() << endl;
    cout << j["html"].string_value() << endl;
  }
}

int main() {
  test_top_sites();
}
