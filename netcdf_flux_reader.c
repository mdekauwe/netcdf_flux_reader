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
//              To get around static arrays
// http://www.unidata.ucar.edu/support/help/MailArchives/netcdf/msg08338.html

#include "netcdf_flux_reader.h"

int main(int argc, char **argv) {
    int   i, jy, kx;
    long  offset;
    char  infname[STRING_LENGTH];

    float *data_in = NULL;

    int  status, nc_id, var_id;

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


    jy = 0; //
    kx = 0;
    for (i = 0; i < NT; i++) {

        offset = ((i * NY + jy) * NX) + kx;
        printf("%f\n", data_in[offset]);

    }


    free(c);

    return(EXIT_SUCCESS);

}

void read_nc_file_into_array(control *c, char *infname, float *nc_in) {

    int  status, nc_id, var_id;

    if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
        ERR(status);
    }

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
