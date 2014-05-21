#include <iostream>

#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "protocol.h"

using namespace std;

const int SERVER_PORT = 9000;
const char *SERVER_HOST = "127.0.0.1";

void put(int sockfd, const char *key, int key_len,
	 const char* value, int value_len) {
	Head head = Head::makePut(key_len, value_len);
	char *buff = new char[head.bufferSize()];
	head.copyTo(buff, key, value);
	send(sockfd, buff, head.bufferSize(), 0);
	delete[] buff;
}

void put_str(int sockfd, const char *key, const char *value) {
	put(sockfd, key, strlen(key) + 1, value, strlen(value) + 1);
}

int main() {
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

	put_str(sockfd, "Hello", "World");
//	const char *text = "some text";
//	send(sockfd, text, strlen(text), 0);
}
