#ifndef GET_LT_AUSSIE_TMAX_H
#define GET_LT_AUSSIE_TMAX_H

#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <strings.h>
#include <string.h>

#define ERRCODE 2
#define ERR(e) {printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE);}
#define NT 70128
#define NY 1
#define NX 1

#define STRING_LENGTH 2000
#define TRUE 1
#define FALSE 0

#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))

typedef struct  {
    char   fdir[STRING_LENGTH];
    char   var_name[STRING_LENGTH];
    int    window;
    int    start_yr;
    int    end_yr;
} control;


void query_nc_dims(int, long *, long *, long *);
void read_nc_file_into_array(control *, char *, float *nc_in);


#endif /* GET_LT_AUSSIE_TMAX_H */
