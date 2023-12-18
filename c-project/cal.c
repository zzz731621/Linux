#include <stdio.h>
#include <stdlib.h>

int main( int argc, char *argv[] ) {

    float ap = atof( argv[1] );
    float sm = atof( argv[2] );
    float de = atof( argv[3] );
    float pro = 0.6;
    float att = 520;

    float min_res, max_res, min, max;

    min_res = ( ap * 3.3 * pro * 0.9 + ap * ap * 0.003365 + sm * 0.5 ) * att / 100 - de / 3;
    max_res = ( ap * 3.3 + ap * ap * 0.003365 + sm * 0.5 ) * att / 100 - de / 3;


    min = ( ap * 3.3 * pro * 0.9 + ap * ap * 0.003365 + sm * 0.5 ) * att / 100 * 1.5 - de / 3;
    max = ( ap * 3.3 + ap * ap * 0.003365 + sm * 0.5 ) * att / 100 * 1.5 - de / 3;


    printf( "min : %f\n", min_res );
    printf( "max : %f\n", max_res );    

    printf( "res : %f\n", min );
    printf( "res : %f\n", max );
    return 0;
} // int