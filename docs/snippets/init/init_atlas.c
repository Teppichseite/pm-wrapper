#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

int main(int argc, char const *argv[])
{
    NVM_Initialize();
    uint32_t id = NVM_FindOrCreateRegion("test", O_RDWR, NULL);

    NVM_CloseRegion(id);
    NVM_Finalize();
    return 0;
}
