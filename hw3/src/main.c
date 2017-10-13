#include <stdio.h>
#include "sfmm.h"
#include "feazan_funk.h"

int main(int argc, char const *argv[])
{

    sf_mem_init();

    double* ptr = sf_malloc(sizeof(double));
    int* ptr2 = sf_malloc(sizeof(int));
    char* ptr3 = sf_malloc(sizeof(char));
    //sf_malloc(sizeof(double));
    //sf_blockprint(&seg_free_list[3].head->header);
    //*ptr = 320320320e-320;
    printf("The double: %f\n", *ptr);
    printf("The int: %d\n", *ptr2);
    printf("The char: %c\n", *ptr3);

    //sf_free(ptr);

    //sf_mem_fini();

    return EXIT_SUCCESS;
}
