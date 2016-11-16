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
    float *data_in = NULL;


    // allocate some memory
    control *c;
    c = (control *)malloc(sizeof (control));
    if (c == NULL) {
        fprintf(stderr, "control structure: Not allocated enough memory!\n");
        exit(EXIT_FAILURE);
    }



    if ((data_in = (float *)calloc(NT * NY * NX, sizeof(float))) == NULL) {
        fprintf(stderr, "Error allocating space for data_in array\n");
        exit(EXIT_FAILURE);
    }

    // Initial values, these can be changed on the cmd line
    strcpy(c->fdir, "/Users/mdekauwe/src/c/netcdf_flux_reader");
    strcpy(c->var_name, "Tair");

    sprintf(infname, "%s/TumbaFluxnet.1.4_met.nc", c->fdir);

    read_nc_file_into_array(c, infname, &(*data_in));

    for (i = 0; i < NT; i++) {

        offset = ((i * NY + jy) * NX) + kx;
        printf("%f\n", data_in[offset]);

    }


    free(c);

    return(EXIT_SUCCESS);

}

void query_nc_header(int nc_id, int *ntime, int *ny, int *nx) {

    int  status;

    // Query dims
    if ((status = nc_inq_dimid(nc_id, "time", ntime))) {
        ERR(status);
    }

    if ((status = nc_inq_dimid(nc_id, "y", ny))) {
        ERR(status);
    }

    if ((status = nc_inq_dimid(nc_id, "x", nx))) {
        ERR(status);
    }



    return;
}


void read_nc_file_into_array(control *c, char *infname, float *nc_in) {

    int  status, nc_id, var_id, ny, nx, ntime;
    int blah;
    // Open the netcdf file
    if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
        ERR(status);
    }
    

    query_nc_header(nc_id, &ntime, &ny, &nx);
    printf("%d %d %d\n", ntime, ny, nx);
    exit(1);





    if ((status = nc_inq_varid(nc_id, c->var_name, &var_id))) {
        ERR(status);
    }

    if ((status = nc_get_var_float(nc_id, var_id, nc_in))) {
        ERR(status);
    }

    if ((status = nc_close(nc_id))) {
        ERR(status);
    }

    return;
}
