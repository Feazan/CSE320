#include <stdio.h>
#include "sfmm.h"
#include "feazan_funk.h"

int main(int argc, char const *argv[])
{

    sf_mem_init();

    /*double *ptr = sf_malloc(sizeof(double));
    int *ptr2 = sf_malloc(sizeof(int));
    char *ptr3 = sf_malloc(sizeof(char));*/
    int *big_ptr = sf_malloc(5000);

    /**ptr = 1.2;
    *ptr2 = 2;
    *ptr3 = 'c';*/

    *big_ptr = 5000;

    //sf_malloc(sizeof(double));
    //sf_blockprint(&seg_free_list[3].head->header);
    //*ptr = 320320320e-320;

    /*printf("The double: %f\n", *ptr);
    printf("The int: %d\n", *ptr2);
    printf("The char: %c\n", *ptr3);*/
    printf("Big ptr: %c\n", *big_ptr);

    sf_blockprint((char *) big_ptr -8);

    //sf_free(ptr);

    //sf_mem_fini();

    return EXIT_SUCCESS;
}
