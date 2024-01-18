#ifdef __cplusplus
extern "C" {
#endif 

#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <errno.h>
#include <stdarg.h>

#define	log_error(...)	do { FILE * fp_log = fopen("log/logg.txt", "a"); if ( errno ) {printf("error:%s.%s:%d %s ", __FILE__, __func__, __LINE__, strerror(errno)); errno = 0;} fprintf(fp_log, __VA_ARGS__); printf(__VA_ARGS__); fclose(fp_log); } while(0)
#define log(...)		do { FILE * fp_log = fopen("log/logg.txt", "a"); fprintf(fp_log, __VA_ARGS__); printf(__VA_ARGS__); fclose(fp_log); } while(0)

#endif

#ifdef __cplusplus
}
#endif