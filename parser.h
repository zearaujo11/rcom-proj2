#ifndef _PARSER_H_
#define _PARSER_H_

#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
void parse_url(char* url, char** user, char** password, char** host, char** url_path, char** file_name);


#endif