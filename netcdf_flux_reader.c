//
//
// PROGRAM:
//              netcdf_flux_reader
//
// DESCRIPTION:
//              Read a netcdf flux file into C array
//
// AUTHOR:      Martin De Kauwe
//
// EMAIL:       mdekauwe@gmail.com
//
// DATE:        16th November, 2016
//
// Notes:
//              To get around netcdf interface we need to use a single
//              contiguous block of memory
//              So a 3D array float array[A][B][C] would then need some offset
//              logic to access it from a single block, e.g.
//              offset = ((i * B + j) * C + k), where i,j,k refer to A,B,C
//              see, but note my ny and nx dims are reversed for the PALS exp
// http://www.unidata.ucar.edu/support/help/MailArchives/netcdf/msg08338.html

#include "netcdf_flux_reader.h"

int main(int argc, char **argv) {
    int    i, status, nc_id, var_id;
    int    jy = 0, kx = 0; // I'm just setting these for completness

    long   offset;
    char   infname[STRING_LENGTH];

    control *c;
    met_arrays *ma;

    // allocate some memory
    c = (control *)malloc(sizeof (control));
    if (c == NULL) {
        fprintf(stderr, "control structure: Not allocated enough memory!\n");
        exit(EXIT_FAILURE);
    }

    ma = (met_arrays *)malloc(sizeof(met_arrays));
    if (ma == NULL) {
    	fprintf(stderr, "met arrays structure: Not allocated enough memory!\n");
    	exit(EXIT_FAILURE);
    }

    // Initial values, these can be changed on the cmd line
    strcpy(c->fdir, "/Users/mdekauwe/src/c/netcdf_flux_reader");
    sprintf(infname, "%s/TumbaFluxnet.1.4_met.nc", c->fdir);
    c->sub_daily = TRUE;


    read_met_data_from_netcdf_file(c, ma, infname);


    for (i = 0; i < NT; i++) {

        offset = ((i * NY + jy) * NX) + kx;
        printf("%f\n", i, ma->tair[offset]);

    }


    free(c);

    free(ma);

    return(EXIT_SUCCESS);

}

void query_nc_dims(int nc_id, long *ntime, long *ny, long *nx) {

    int  status, time_id, y_id, x_id;
    long length;

    if ((status = nc_inq_dimid(nc_id, "time", &time_id))) {
        ERR(status);
    }

    if ((status = nc_inq_dimlen(nc_id, time_id, ntime))) {
        ERR(status);
    }

    if ((status = nc_inq_dimid(nc_id, "y", &y_id))) {
        ERR(status);
    }

    if ((status = nc_inq_dimlen(nc_id, y_id, ny))) {
        ERR(status);
    }

    if ((status = nc_inq_dimid(nc_id, "x", &x_id))) {
        ERR(status);
    }

    if ((status = nc_inq_dimlen(nc_id, x_id, nx))) {
        ERR(status);
    }

    return;
}


void read_met_data_from_netcdf_file(control *c, met_arrays *ma, char *infname) {

    int  status, nc_id, tair_id;
    long ny, nx, ntime;

    // Open the netcdf file
    if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
        ERR(status);
    }

    query_nc_dims(nc_id, &ntime, &ny, &nx);

    // Allocate met arrays
    if ((ma->tair = (double *)calloc(ntime, sizeof(double))) == NULL) {
        fprintf(stderr,"Error allocating space for tair array\n");
		exit(EXIT_FAILURE);
    }

    if ((status = nc_inq_varid(nc_id, "Tair", &tair_id))) {
        ERR(status);
    }

    if ((status = nc_get_var_double(nc_id, tair_id, &(*ma->tair)))) {
        ERR(status);
    }

    if ((status = nc_close(nc_id))) {
        ERR(status);
    }

    return;
}
