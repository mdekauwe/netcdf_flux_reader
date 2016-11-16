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

#include "netcdf_flux_reader.h"

int main(int argc, char **argv) {
    int   i;
    long  offset;
    char  infname[STRING_LENGTH];

    // Need to be declared like this otherwise the netcdf read will attempt to
    // use the heap rather than the stack to allocate memory and run out.
    // I'm sure there is a way to get netcdf to read into a 1D array but I
    // don't know how to do that and can't find a quick example
    // NB. I'm declaring 1 extra spot for leap years, so we will need to make
    // sure when we read from this array we are checking leap years.
    static float data_in[NT][NY][NX];

    // allocate some memory
    control *c;
    c = (control *)malloc(sizeof (control));
    if (c == NULL) {
        fprintf(stderr, "control structure: Not allocated enough memory!\n");
        exit(EXIT_FAILURE);
    }

    // Initial values, these can be changed on the cmd line
    strcpy(c->fdir, "/Users/mdekauwe/src/c/netcdf_flux_reader");
    strcpy(c->var_name, "Tair");

    sprintf(infname, "%s/TumbaFluxnet.1.4_met.nc", c->fdir);

    read_nc_file_into_array(c, infname, data_in);

    for (i = 0; i < NT; i++) {
        printf("%f\n", data_in[i][0][0]);
    }


    free(c);

    return(EXIT_SUCCESS);

}

void read_nc_file_into_array(control *c, char *infname,
                             float nc_in[NT][NY][NX]) {

    int  status, nc_id, var_id;

    if ((status = nc_open(infname, NC_NOWRITE, &nc_id))) {
        ERR(status);
    }

    if ((status = nc_inq_varid(nc_id, c->var_name, &var_id))) {
        ERR(status);
    }

    if ((status = nc_get_var_float(nc_id, var_id,
                                   &nc_in[0][0][0]))) {
        ERR(status);
    }

    if ((status = nc_close(nc_id))) {
        ERR(status);
    }

    return;
}
