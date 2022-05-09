#include <stdio.h>
#include "../../backends/pmdk_backend.h"
#include "multiple_region_test.h"

int main(int argc, char const *argv[])
{
    printf("\x1B[33mRunning multiple region test for PMDK\x1B[0m\n");
    multiple_region_test(&PMDK_BACKEND);
    return 0;
}
