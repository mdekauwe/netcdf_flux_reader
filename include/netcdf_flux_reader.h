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
    int    sub_daily;
} control;

typedef struct {
    double *year;
    double *rain;
    double *par;
    double *tair;
    double *tsoil;
    double *co2;
    double *ndep;
    double *nfix;       /* N inputs from biological fixation (t/ha/timestep (d/30min)) */
    double *wind;
    double *press;

    /* Day timestep */
    double *prjday; /* should really be renamed to doy for consistancy */
    double *tam;
    double *tpm;
    double *tmin;
    double *tmax;
    double *tday;
    double *vpd_am;
    double *vpd_pm;
    double *wind_am;
    double *wind_pm;
    double *par_am;
    double *par_pm;

    /* sub-daily timestep */
    double *vpd;
    double *doy;
    double *diffuse_frac;

} met_arrays;


void query_nc_dims(int, long *, long *, long *);
void read_met_data_from_netcdf_file(control *, met_arrays *ma, char *);


#endif /* GET_LT_AUSSIE_TMAX_H */
