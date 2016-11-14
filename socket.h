#ifdef __cplusplus
extern "C" {
#endif 

#ifndef LOGGER_H
#define LOGGER_H

#define	log_error(...)	do { FILE * fp_log = fopen("log/logg.txt", "a"); if ( errno ) {printf("error:%s.%s:%d %s ", __FILE__, __func__, __LINE__, strerror(errno)); errno = 0;} fprintf(fp_log, __VA_ARGS__); printf(__VA_ARGS__); fclose(fp_log); } while(0)
#define log(...)		do { FILE * fp_log = fopen("log/logg.txt", "a"); fprintf(fp_log, __VA_ARGS__); printf(__VA_ARGS__); fclose(fp_log); } while(0)

#endif

#ifndef SOCKET_HANDLER_H
#define SOCKET_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>
#include <pthread.h>

#define SOCKET_INACTIVE			-1
#define PORT_WEB				8088
#define PORT_STERAM				9090


void open_jpeg_socket(void);
void open_web_socket(void);
void close_jpeg_socket(void);
void close_web_socket(void);

#endif

#ifdef __cplusplus
}
#endif