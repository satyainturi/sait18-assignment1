#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;

#define PROTOCAL "TEXT TCP 1.0"

struct calc_data {
	string str;
	double fval1, fval2;
	int val1, val2;
};

void calc(char* calc_string, char* result);
int  check_protocols(char*);
void fail();
string get_address_and_port(char* arg, int& port);
void print_help();
int  get_listen_port(char* arg);

int main(int argc, char** argv) {
	
	if (argc < 2) {
		print_help();
	}

	int port = 0;
	string address = get_address_and_port(argv[1], port);
	if (address.empty()) {
		print_help();
	}
	

	// socket create and verification 
	int sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		cout << "client - socket creation failed..." << endl; 
		exit(0); 
	} 
	else {
		cout << "client - socket created.." << endl; 
	}
	
	struct sockaddr_in servaddr; 
	memset(&servaddr, 0, sizeof(servaddr)); 
	
	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(address.c_str()); 
	servaddr.sin_port = htons(port); 
	int ret	 = connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
	cout << "client - connected to server: " << ret << endl;
	
	//receive the string from server and test the string
	char buf[1024];
	memset(buf, 0, sizeof(buf));
	int len = recv(sockfd, buf, sizeof(buf), 0);	
	if (len < 0) {
		fail();
	}
	
	cout << "client - protocals:" << PROTOCAL << endl;
	cout << "client - server protocals:" << endl << buf << endl;
	if (check_protocols(buf) == 0) {
		fail();
	}
	len = send(sockfd, "OK", 2, 0);
	if (len < 0) {
		fail();
	}	

	//receive cal string
	char calc_string[50];
	memset(calc_string, 0, sizeof(calc_string));

	len = recv(sockfd, calc_string, sizeof(calc_string), 0);
	if (len < 0) {
		fail();
	}
	cout << "client - server said " << calc_string << endl;

	char result[20];
	memset(result, 0, sizeof(result));
	calc(calc_string, result); 
	len = send(sockfd, result, strlen(result), 0);
	cout << "client - send result: " << result << endl;
	if (len < 0) {
		fail();
	}

	char response[50];
	memset(response, 0, sizeof(response));
	len = recv(sockfd, response, sizeof(response), 0);
	if (len < 0) {
		fail();
	}
	cout << "client - server reply is " << response << endl;
	// After chatting close the socket 
	close(sockfd); 
  
	return 0;
}

void fail() {
	cout << "client - connection is failed" << endl;
	exit(0);
}

void print_help() {
	cout << endl << "hel" << endl;
	cout << " client.exe 192.168.1.2:8080" << endl << endl;
	exit(0);
}

string get_address_and_port(char* arg, int& port) {
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
	return address;
}

int check_protocols(char* string) {
	char *ptr = string;
	char temp[20];
	
	while(*ptr != 0) {
		ptr = strchr(string, '\n');
		if (ptr == 0) {
			break;
		}
		
		memset(temp, 0, sizeof(temp));
		strncpy(temp, string, ptr - string);
		if (strcmp(temp, PROTOCAL) == 0) {
			return 1;
		}
		string = ptr + 1;
	}
	return 0;
}

struct calc_data split_calc_string(char* calc_string) {
	struct calc_data cs;
	char text[80] = {0};
	if (calc_string[0] == 'f') {
		sscanf(calc_string, "%s %lg %lg", text, &cs.fval1, &cs.fval2);
	}
	else {
		sscanf(calc_string, "%s %d %d", text, &cs.val1, &cs.val2);
	}
	cs.str = string(text);
	return cs;
}

double get_int_result(const string& str, int value1, int value2) {
	if (str == "add") {
		return (double)(value1 + value2);
	}
	else if (str == "mul") {
		return (double)(value1 * value2);
	}
	else if (str == "sub") {
		return (double)(value1 - value2);
	}
	else if (str == "div") {
		return (double)value1 / (double)value2;
	}
	return 0.0;
}

double get_double_result(const string& str, double value1, double value2) {
	if (str == "fadd") {
		return value1 + value2;
	}
	else if (str == "fmul") {
		return value1 * value2;
	}
	else if (str == "fsub") {
		return value1 - value2;
	}
	else if (str == "fdiv") {
		return value1 / value2;
	}
	return 0.0;
}

void calc(char* calc_string, char* result) {
	cout << "client - calc..." << endl;
	
	struct calc_data cs = split_calc_string(calc_string);	

	if (cs.str.size() == 4) {
		double double_result = get_double_result(cs.str, cs.fval1, cs.fval2);
		sprintf(result, "%.8g", double_result);
	}
	else {
		double int_result = get_int_result(cs.str, cs.val1, cs.val2);
		sprintf(result, "%.8g", int_result);
	}
	cout << "client - completed" << endl;

}