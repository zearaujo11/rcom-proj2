#include "parser.h"

void parse_url(char* url, char** user, char** password, char** host, char** url_path, char** file_name){
    char* host_and_path;

    if(strchr(url, '@') ){
        if(sscanf(url,"ftp://%m[^:]:%m[^@]@%ms",user,password,&host_and_path)<3){
            printf("Wrong URL\n Example: ftp://[<user>:<password>@]<host>/<url-path>\n");
            exit(-1);
        }
    }
    else{
        if(sscanf(url,"ftp://%ms",&host_and_path)==1){
            *user = (char*)malloc(10*sizeof(char));
            if(!user){
                perror("failed allocating memory for username");
                exit(-1);
            }
            strcpy(*user, "anonymous");

            *password = (char*)malloc(sizeof(char));
            if(!password){
                perror("failed allocating memory for password");
                exit(-1);
            }
            strcpy(*password, "");
        }
        else{
            printf("Wrong URL\n Example: ftp://[<user>:<password>@]<host>/<url-path>\n");
            exit(-1);
        }
    }


    if(sscanf(host_and_path,"%m[^/]/%ms",host,url_path)<2){
        printf("Wrong URL\n Example: ftp://[<user>:<password>@]<host>/<url-path>\n");
        exit(-1);
    }
    *file_name = basename(*url_path);
}