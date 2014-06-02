#include <iostream>
#include <fstream>
#include <map>

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <time.h>
#include <netinet/in.h>
#include <unistd.h>

#include "protocol.h"
#include "json11.hpp"
#include "constants.h"

using namespace std;
using namespace json11;


const int SERVER_PORT = 9000;
const char *SERVER_HOST = "127.0.0.1";

const int RESPONSE_BUFF_SIZE = MAX_BUFFER_SIZE;
const int VALUE_BUFF_SIZE = MAX_BUFFER_SIZE;

char result_buffer[MAX_BUFFER_SIZE];
char check_key_buffer[MAX_BUFFER_SIZE];
char check_value_buffer[MAX_BUFFER_SIZE];

int put(int sockfd, const char *key, int key_len,
	 const char* value, int value_len) {
	Head head = Head::makePut(key_len, value_len);
	char *buff = new char[head.bufferSize()];

	head.makePackage(buff, key, value);

	head.extract(check_key_buffer, check_value_buffer, buff);
	cout << "client put: " << strlen(value) << " vs " << value_len
	     << " vs " << strlen(check_value_buffer) << endl;
	assert(strcmp(value, check_value_buffer) == 0);
	send(sockfd, buff, head.bufferSize(), 0);
	cout << "BufferSize" << head.bufferSize() << endl;
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

void put(int sockfd, const string &key, const string &value) {
	put_str(sockfd, key.c_str(), value.c_str());
}

int get_str(int sockfd, const char *key, char *value) {
	return get(sockfd, key, strlen(key) + 1, value);
}

string get(int sockfd, string key) {
	int len = get_str(sockfd, key.c_str(), result_buffer);
	if (len != -1) {
		return string{result_buffer};
	} else {
		return string{""};
	}
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

void put_pairs(const map<string, string> pairs) {
	for (auto e : pairs) {
		string key = e.first;
		string value = e.second;
		int sockfd = connectSocket();
		put(sockfd, key, value);
		close(sockfd);
	}
}

void get_pairs(const map<string, string> &hot,
	       const map<string, string> &cold,
	       int count, double hot_rate) {
	while (count > 0) {
		int sockfd = connectSocket();
		double r = (double) rand() / RAND_MAX;
		string key;
		string value;
		if (r < hot_rate) {
			int index = rand() % hot.size();
			auto item = hot.begin();
			advance(item, index);
			key = item->first;
			value = item->second;
		} else {
			int index = rand() % cold.size();
			auto item = cold.begin();
			advance(item, index);
			key = item->first;
			value = item->second;
		}
		string value_back = get(sockfd, key);
		close(sockfd);

		// if (value_back != value) {
		// 	cout << "put: " << value.size() << endl;
		// 	cout << "get: " << value_back.size() << endl;
		// }
		// assert(value_back == value);
		--count;
	}
}

void test_top_sites(int hot_copies = 1, int cold_copies = 1,
		    double hot_rate = 0.5, int count = 1000) {
        // max value length: 289208
	string text = read_file("tools/sites.json");
	string error;
	Json parsed = Json::parse(text, error);
	assert(parsed.is_array());
	Json::array arr = parsed.array_items();

	map<string, string> hot;
	map<string, string> cold;
	for (Json j : arr) {
		assert(j.is_object());
		string key = j["site"].string_value();
		string value = j["html"].string_value();
		if (hot_copies == 1) {
			hot[key] = value;
		} else {
			for (int i = 0; i != hot_copies; ++i) {
				hot[key + "@" + to_string(i + 1)] = value;
			}
		}
		if (cold_copies == 1) {
			cold[key] = value;
		} else {
			for (int i = 0; i != cold_copies; ++i) {
				cold[key + "@" + to_string(i + 1)] = value;
			}
		}
	}
	put_pairs(hot);
	put_pairs(cold);
	get_pairs(hot, cold, hot_rate, count);
	int sockfd = connectSocket();
	get_stats(sockfd);
	close(sockfd);
}

void test_once() {
	char value_buff[MAX_BUFFER_SIZE];
	int sockfd = connectSocket();
	put(sockfd, "Hello", "World");
	close(sockfd);

	// sockfd = connectSocket();
	// string value = get(sockfd, "Hello");
	// close(sockfd);

	sockfd = connectSocket();
	put(sockfd, "Hi", "Everyone");
	close(sockfd);

	// sockfd = connectSocket();
	// value = get(sockfd, "Hi");
	// close(sockfd);

	sockfd = connectSocket();
	get_stats(sockfd);
}

int main(int argc, char **argv) {
	cout << "client started..." << endl;
	if (argc == 1) {
		test_top_sites();
	} else if (argc == 4) {
		int copies = stoi(string{argv[1]});
		double rate = stod(string{argv[2]});
		int count = stoi(argv[3]);
		test_top_sites(copies, rate, count);
	} else {
		cout << "Usage: ./client <copies> <hot_rate> <get_count>" 
		     << endl;
	}
	//test_once();
}
