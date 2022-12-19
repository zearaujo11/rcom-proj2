#include "parser.h"
#include "connection.h"

#define BUF_SIZE 10000
#define FTP_PORT 21


int main(int argc, char **argv) {

    char receive_buf[BUF_SIZE];
    int server_response;
    

    if (argc != 2){
        printf("Wrong number of arguments\n");
        printf("Usage Example: download ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }

    char *user, *password, *host, *url_path, *file_name;

    parse_url(argv[1],&user,&password,&host,&url_path,&file_name);


    char* user_command = (char*) malloc(strlen(user)+7);
    if(!user_command){
        perror("failed allocating memory for user command");
        exit(-1);
    }
    strcpy(user_command, "user ");
    strcat(user_command, user);
    strcat(user_command, "\n");


    char* password_command = (char*) malloc(strlen(password)+7);
    if(!password_command){
        perror("failed allocating memory for password command");
        exit(-1);
    }
    strcpy(password_command, "pass ");
    strcat(password_command, password);
    strcat(password_command, "\n");



    char* server_addr = getIP(host);



    int sockfd = connect_to_server(server_addr,FTP_PORT);
    server_response = receiveCode(sockfd,receive_buf);

    if(server_response!=220){
        printf("Error connecting to server\n");
        printf("Server response\n%s",receive_buf);
        exit(-1);
    }
    printf("Successfully connected to the server\n");



    //log in


    //username
    send_command(sockfd, user_command);
    server_response = receiveCode(sockfd,receive_buf);

    if(server_response==331){
        //password
        send_command(sockfd, password_command);
        server_response = receiveCode(sockfd,receive_buf);
    }
    if (server_response!=230){
        printf("Error Logging in\n");
        printf("Server response\n%s",receive_buf);
        exit(-1);
    }
    printf("Successfully Logged In\n");



    // Receive file


    // Switch to binary mode
    send_command(sockfd, "TYPE I\n");
    server_response = receiveCode(sockfd,receive_buf);
    if(server_response!=200){
        printf("Error switching to Binary Mode\n");
        printf("Server response\n%s",receive_buf);
        exit(-1);
    }
    printf("Switched to Binary Mode\n");


    // Enter passive mode
    send_command(sockfd, "pasv\n");
    server_response = receiveCode(sockfd,receive_buf);
    if(server_response!=227){
        printf("Error Entering passive mode\n");
        printf("Server response\n%s",receive_buf);
        exit(-1);
    }


    // Get IP and Port
    char ip[16];
    uint16_t port;
    
    if(get_ip_port(receive_buf, ip, &port)){
        printf("Error getting ip address and port when entering passive mode\n");
        close(sockfd);
        exit(-1);
    }
    
    printf("Switched to Passive Mode\nIP Address: %s\nPort:%d\n",ip,port);
    

    // creating socket to receive file
    int receive_socket = connect_to_server(ip,port);
    

    // sending command to receive file
    char* retr_url_path = (char*) malloc(strlen(url_path)+7);

    if(!retr_url_path){
        perror("failed allocating memory for retr command");
        exit(-1);
    }

    strcpy(retr_url_path, "retr ");
    strcat(retr_url_path, url_path);
    strcat(retr_url_path, "\n");
    send_command(sockfd, retr_url_path);
    
    server_response = receiveCode(sockfd,receive_buf);
    if(server_response!=150){
        printf("Error Opening BINARY mode data connection for %s\n",file_name);
        printf("Server response\n%s",receive_buf);
        exit(-1);
    }
    
    printf("Successfully opened BINARY mode data connection for %s\n",file_name);
    
    receive_file(file_name,receive_buf,receive_socket);



    // closing connection
    send_command(sockfd, "QUIT");

    if (close(sockfd)<0) {
        perror("close(sockfd)");
        exit(-1);
    }
    if (close(receive_socket)<0) {
        perror("close(receive_socket)");
        exit(-1);
    }
    
    free(retr_url_path);
    free(user);
    free(user_command);
    free(password);
    free(password_command);
    free(host);
    free(url_path);
    //file_name points to the end of url_path (after the last /) so it was already freed
    
    return 0;
}