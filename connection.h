#ifndef _CONNECTION_H_
#define _CONNECTION_H_

#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int get_ip_port(char receive_buf[], char *ip, uint16_t* port);

int receiveCode(int sockfd,char receive_buf[]);

int connect_to_server(char* server_ip_addr,uint16_t server_port);

int send_command(int sockfd, char * cmd);

char* getIP(char* hostName);

void receive_file(char* file_name, char receive_buf[], int receive_socket);


#endif