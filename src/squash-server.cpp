#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
	int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}
