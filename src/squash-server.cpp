#include <iostream>

#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int PORT = 9000;
const char *HOST = "127.0.0.1";

const int BUFF_SIZE = 4096;

int main() {
	int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(HOST);

	bzero(&(addr.sin_zero), 8);
	cout << bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	char buff[BUFF_SIZE];

	while (true) {
		//recv(sockfd, buff, BUFF_SIZE, 0);
	}
}
