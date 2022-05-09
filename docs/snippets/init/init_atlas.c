#include "../../Atlas/runtime/include/atlas_api.h"
#include "../../Atlas/runtime/include/atlas_alloc.h"

int main(int argc, char const *argv[])
{
    NVM_Initialize();
    int created_new = 0;
    uint32_t id = NVM_FindOrCreateRegion("test", O_RDWR, &created_new);

    if (created_new)
    {
        void *root = nvm_calloc(sizeof(int), 1, id);
        NVM_SetRegionRoot(id, root);
    }

    NVM_CloseRegion(id);
    NVM_Finalize();
    return 0;
}
