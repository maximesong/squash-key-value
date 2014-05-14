#include <iostream>

#include <arpa/inet.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

const int SERVER_PORT = 9000;
const char *SERVER_HOST = "127.0.0.1";

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

	const char *text = "some text";
	send(sockfd, text, strlen(text), 0);
}
