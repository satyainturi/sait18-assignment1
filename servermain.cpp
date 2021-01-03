#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "calcLib.h"

using namespace std;

// Protocals supported in server side.
#define PROTOCALS "TEXT TCP 1.0\nTEXT TCP 1.2\n"

int  print_help();
void process(int connfd);
void get_rand_calc_data(struct calc_data* calc);
string get_address_and_port(char* arg, int& port);

int main(int argc, char** argv) 
{
	cout << "server start" << endl;

	// server MUST be started with listen IP and port.
	if (argc < 2) {
		return print_help();
	}
	
	int port = 0;
	string address = get_address_and_port(argv[1], port);
	if (address.empty()) {
		return print_help();
	}
		
	// socket create and verification 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) {
		cout << "server - socket error." << endl; 
		exit(0); 
	} 
	else {
		cout << "server - successed to create socket." << endl; 
	}
	
	// Forcefully attaching socket to the port 8080 
	int opt = 1; 
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt))) { 
        cout << "server - socket option error" << endl;
        exit(0); 
    } 
	
	int seed = sockfd;
	srand((unsigned int)seed);

	struct sockaddr_in servaddr; 
	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port); 
	
	// Binding newly created socket to given IP
	if ((bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))) != 0) { 
		cout << "server - socket bind error " << strerror(errno) << endl;
		exit(0); 
	} 
	else {
		cout << "server - socket binded" << endl; 
	}

	while (1) 
	{
		cout << "server - listen on " << address << ":" << port; 
		
		if ((listen(sockfd, 5)) != 0) { 
			cout << "server - listen error" << endl; 
			exit(0); 
		}
		
		struct sockaddr_in client; 
		socklen_t len = (socklen_t)sizeof(client); 
		
		// Accept the data packet from client
		int connfd = accept(sockfd, (struct sockaddr*)&client, &len); 
		if (connfd < 0) { 
			cout << "server - accept error" << endl; 
			exit(0);
		} 
		else {
			cout << "server - accepted a client" << endl; 
		}
		
		// Function for messaging between client and server 
		process(connfd); 

		//  close the client socket 
		close(connfd);
	}

	// After chatting close the socket 
	close(sockfd); 
  
	return 0;
}

int print_help() 
{
	cout << endl << "Help" << endl;
	cout << " server.exe localhost:8080 " << endl << endl;
	exit(0);
}

string get_address_and_port(char* arg, int& port) 
{
    string str(arg);
	if (str.size() >= 20) {
		print_help();
	}

    int pos = (int)str.find(":");
    if (pos == string::npos) {
        print_help();
    }

	string address = str.substr(0, pos);
    string sport = str.substr(pos + 1, str.size() - pos - 1);
	port = atoi(sport.c_str());

	if (address == "localhost") address = "127.0.0.1";
	return address;
}

int check_protocal_version(int connfd) 
{
	char protocals[] = PROTOCALS;
	int result = send(connfd, protocals, sizeof(protocals), 0);
	if (result < 0) {
		cout << "server - socket send error 1" << endl;
		exit(0);
	}

	char buf[1024];
	memset(buf, 0, sizeof(buf));

	result = recv(connfd, buf, sizeof(buf), 0);
	if (result < 0) {
		cout << "server - socket recv error 1" << endl;
		exit(0);
	}

	if (strcmp(buf, "OK") != 0) {
		cout << "server - the client doesn't support protocal." << endl;
		return -1;
	}

	return 0;
}

void process(int connfd) {
	// check client's protocal version.
	// if the client accepts the protocol, it should respond with "OK".
	if (check_protocal_version(connfd) != 0) {
		cout << "server - the client doesn't support protocal." << endl;
		return;
	}

	// create calc_data to be sent to the client.
	struct calc_data calc;
	create_calc_data(&calc);

	char msg[400];
	get_calc_data_string(&calc, msg);
	cout  << "server - message: " << msg;

	int result = send(connfd, msg, strlen(msg) + 1, 0);
	if (result < 0) {
		cout << "server - socket send error" << endl;
		return;
	}

	memset(msg, 0, sizeof(msg));
	result = recv(connfd, msg, sizeof(msg), 0);
	if (result < 0) {
		cout << "server - socket recv error" << endl;
		return;
	}
	cout << "server - received: " << msg << endl;

	double server_val = calculate(&calc);
	double client_val = 0.0;
	sscanf(msg, "%lg", &client_val);

	string response = "OK";
	if (server_val != client_val) {
		//There is a issue in double operation. 
		//Double operation result is sligty different every computers.
		if (calc.str.find("f") == 0) {
			if (fabs(server_val - client_val) > 1) {
				response = "ERROR";
			}
		}
	}
	cout << "server - compare result is " << response << endl;
	
	result = send(connfd, response.c_str(), response.size() + 1, 0);
	if (result < 0) {
		cout << "server - socket error" << endl;
		return;
	}
}

