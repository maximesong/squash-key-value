#include <iostream>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "protocol.h"
#include "simple_store.h"

using namespace std;

const int PORT = 9000;
const char *HOST = "127.0.0.1";

const int BUFF_SIZE = 4096;

const int LISTEN_LENGTH = 10;

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(HOST);

	bzero(&(addr.sin_zero), 8);
	bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	char buff[BUFF_SIZE];
	char key[BUFF_SIZE];
	char value[BUFF_SIZE];

	listen(sockfd, LISTEN_LENGTH);
	int fd;

	SimpleStore store;
	Head ok_head = Head::makeOk();
	Head response_head;

	int value_len;
	while ((fd = accept(sockfd, 0, 0))) {
		int len = recv(fd, buff, BUFF_SIZE, 0);
		if (len >= 0) {
			//cout << len << endl;
			Head head = Head::from(buff);
			switch(head.getMethod()) {
			case Head::PUT:
				head.extract(key, value, buff);
				cout << "Key: " <<
					key << endl;
				cout << "Value: " <<
					value << endl;
				store.put_str(key, value);
				ok_head = Head::makeOk();
				ok_head.makePackage(buff);
				send(fd, buff, ok_head.bufferSize(), 0);
				break;
			case Head::GET:
				head.extract(key, buff);
				cout << "Get Key: " <<
					key << endl;
				value_len = store.get_str(key, value);
				if (value_len >= 0) {
					response_head = 
						Head::makeOk(value_len);
					response_head.makePackage(
						buff, value);
					send(fd, buff, 
					     response_head.bufferSize(),
					     0);
				} else {
					response_head =
						Head::makeMiss();
					response_head.makePackage(
						buff);
					send(fd, buff,
					     response_head.bufferSize(),
					     0);
				}
				break;
			default:
				cout << "Unknown" << endl;
			}
		} else {
			cout << "Error: " << len << endl;
		}
	}
}
