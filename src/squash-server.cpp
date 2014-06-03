#include <iostream>
#include <fstream>

#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "protocol.h"
#include "simple_store.h"
#include "compressed_store.h"
#include "complex_store.h"
#include "resource_monitor.h"
#include "constants.h"

#define STORE_MODE 1

using namespace std;

const int PORT = 9000;
const char *HOST = "127.0.0.1";

const char *PID_FILENAME="server.pid";

const int LISTEN_LENGTH = 10;

void write_pid() {
	fstream fout(PID_FILENAME, ios_base::out | ios_base::trunc);
	fout << getpid() << endl;
}

void remove_pid() {
	remove(PID_FILENAME);
}

int main() {
	write_pid();
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(HOST);

	bzero(&(addr.sin_zero), 8);
	int r = bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	if (r != 0) {
		cout << "Failed to bind server" << endl;
		char error[MAX_BUFFER_SIZE];
		perror(error);
		cout << error;
		return -1;
	} else {
		cout << "Starting server..." << endl;
	}
	char buff[MAX_BUFFER_SIZE];
	char key[MAX_BUFFER_SIZE];
	char value[MAX_BUFFER_SIZE];

	listen(sockfd, LISTEN_LENGTH);
	int fd;

	SimpleStore store;
//	CompressedStore store;
	//ComplexStore store;

	Head ok_head = Head::makeOk();
	Head response_head;

	int value_len;
	int usage;
	while ((fd = accept(sockfd, 0, 0))) {
		int size = 0;
		int len = recv(fd, buff, MAX_BUFFER_SIZE, 0);
		Head head;
		if (len != 0) {
			head = Head::from(buff);
			size += len;
		}
		while (len != 0 && size < head.bufferSize()) {
			len = recv(fd, buff + size, MAX_BUFFER_SIZE, 0);
			size += len;
		}
		if (size >= 0) {
			switch(head.getMethod()) {
			case Head::PUT:
				head.extract(key, value, buff);
				cout << "Put Size: " << head.getValueLength() << endl;
				store.put(key, head.getKeyLength(), value, head.getValueLength());
				ok_head = Head::makeOk();
				ok_head.makePackage(buff);
				send(fd, buff, ok_head.bufferSize(), 0);
				close(fd);
				break;
			case Head::GET:
				head.extract(key, buff);
				value_len = store.get_str(key, value);
				if (value_len >= 0) {
					response_head = 
						Head::makeOk(value_len);
					response_head.makePackage(
						buff, value);
					send(fd, buff, 
					     response_head.bufferSize(),
					     0);
					close(fd);
				} else {
					response_head =
						Head::makeMiss();
					response_head.makePackage(
						buff);
					send(fd, buff,
					     response_head.bufferSize(),
					     0);
					close(fd);
				}
				break;
			case Head::STATS:
				usage = ResourceMonitor::getMemoryUsage();
				cout << "Usage:" << usage << endl;
				response_head = Head::makeOk(4);
				response_head.makePackage(
					buff, usage);
				send(fd, buff, 
				     response_head.bufferSize(),
				     0);
				close(fd);
				break;
			default:
				cout << "Unknown" << endl;
				break;
			}
		} else {
			cout << "Error: " << len << endl;
		}
	}
	remove_pid();
}

