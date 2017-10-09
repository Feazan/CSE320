#include <stdio.h>
#include "sfmm.h"
#include "feazan_funk.h"

int main(int argc, char const *argv[])
{

    sf_mem_init();


     //sf_snapshot();
     //sf_blockprint(&seg_free_list[3].head->header);

    //double* ptr = sf_malloc(sizeof(double));
    sf_malloc(sizeof(double));
    //sf_blockprint(&seg_free_list[3].head->header);
    //*ptr = 320320320e-320;

    //printf("%p\n", ptr);
    // printf("%f\n", *ptr);

    //sf_free(ptr);

    //sf_mem_fini();

    return EXIT_SUCCESS;
}
