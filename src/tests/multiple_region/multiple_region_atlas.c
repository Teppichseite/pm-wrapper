#include <stdio.h>
#include "../../backends/atlas_backend.h"
#include "multiple_region_test.h"

int main(int argc, char const *argv[])
{
    printf("\x1B[33mRunning multiple region test for Atlas\x1B[0m\n");
    multiple_region_test(&ATLAS_BACKEND);
    return 0;
}
