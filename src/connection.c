#include "connection.h"

int get_ip_port(char receive_buf[], char *ip, uint16_t* port){
    int ip_1,ip_2,ip_3,ip_4,port_msb,port_lsb;

    if(sscanf(receive_buf,
    "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)",&ip_1,&ip_2,&ip_3,&ip_4,&port_msb,&port_lsb)<6)
        return -1;
    
    if(!snprintf(ip,16,"%d.%d.%d.%d",ip_1,ip_2,ip_3,ip_4)) return -1;

    *port = port_msb*256+port_lsb;

        
    return 0;
}


int receiveCode(int sockfd,char receive_buf[]){
    size_t bytes_rcv = 0;
    int code = 0;
    while((bytes_rcv = read(sockfd,receive_buf,10000))){
        receive_buf[bytes_rcv]='\0';
        char *bg_line = receive_buf;

        for(int i = 0; i < bytes_rcv; i=strchr(bg_line,'\n')-receive_buf){
            char number[4];
            strncpy(number, bg_line, 4);
            code = atoi(number); 
        
            if(bg_line[3] == ' '){
                return code;
            }
            bg_line = &receive_buf[i+1];
        }
    };

    return 0;
}

int connect_to_server(char* server_ip_addr,uint16_t server_port){
    /*server address handling*/
    int fd;
    struct sockaddr_in server_addr;
    bzero((char *) &server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);        /*server TCP port must be network byte ordered */
    server_addr.sin_addr.s_addr = inet_addr(server_ip_addr);    /*32 bit Internet address network byte ordered*/
    

    /*open a TCP socket*/
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket()");
        exit(-1);
    }
    /*connect to the server*/
    if (connect(fd,
                (struct sockaddr *) &server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect()");
        exit(-1);
    }

    return fd;
}



int send_command(int sockfd, char * cmd){
    return write(sockfd, cmd, strlen(cmd));
}


char* getIP(char* hostName){
    struct hostent *h;

    if ((h = gethostbyname(hostName)) == NULL) {
        herror("Invalid Host name");
        exit(-1);
    }
    return  inet_ntoa(*((struct in_addr *) h->h_addr));
}


void receive_file(char* file_name, char receive_buf[], int receive_socket){
    FILE * file = fopen(file_name, "wb");

    if(file == NULL){
        perror("cannot open file");
        exit(-1);
    }

    size_t bytes_rcv;
    while((bytes_rcv = read(receive_socket,receive_buf,10000))){
        receive_buf[bytes_rcv]='\0';
        if(fwrite(receive_buf , 1 , bytes_rcv , file ) < bytes_rcv){
            perror("Error writing to file file");
            exit(-1);
        }
    }


    if(fclose(file)){
        perror("cannot close file");
        exit(-1);
    }

    printf("File successfully saved in %s\n",file_name);
}