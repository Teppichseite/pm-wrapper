#include <stdio.h>
#include "pm_wrapper.h"

static PmWrapperConfig config;
static PmBackend backend;

int pm_init(PmWrapperConfig pm_config, PmBackend pm_backend)
{
    config = pm_config;
    backend = pm_backend;

    if (backend.open(config) == 0)
    {
        return 0;
    }

    return backend.create(config);
}

void *pm_get_root()
{
    return (void *)backend.get_root();
};

void *pm_alloc(int size)
{
    return (void *)backend.malloc(size);
}

void *pm_read_object(void *ptr)
{
    return backend.read_object((pm_offset)ptr);
}

void pm_write_object(void *ptr, char *data, int len)
{
    backend.write_object(ptr, data, len);
};

int pm_close()
{
    return backend.close(config);
};
