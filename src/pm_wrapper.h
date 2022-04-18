#ifndef PM_WRAPPER_H
#define PM_WRAPPER_H
#include <stdlib.h>

typedef __UINT64_TYPE__ pm_region_offset;
typedef __UINT64_TYPE__ pm_region_id;

typedef __UINT16_TYPE__ pm_region_reference_id;

typedef union PmThinPointer
{
    struct
    {
        pm_region_offset offset : 48;
        pm_region_reference_id reference_id : 16;
    };
    void *ptr;
} PmThinPointer;

typedef struct PmMainRegionRoot
{
    pm_region_offset program_root;
    pm_region_offset region_id_table;
} PmMainRegionRoot;

typedef struct PmRegionConfig
{
    pm_region_reference_id reference_id;
    char *file_path;
    size_t root_size;
} PmRegionConfig;

typedef struct PmBackend PmBackend;

typedef struct PmWrapperConfig
{
    PmBackend *backend;
    PmRegionConfig main_region_config;
} PmWrapperConfig;

typedef struct PmBackendContext
{
    PmRegionConfig region_config;
    pm_region_reference_id reference_id;
    pm_region_id id;
    void *data;
} PmBackendContext;

struct PmBackend
{
    int (*open)(PmBackendContext *context);
    int (*create)(PmBackendContext *context);
    int (*close)(PmBackendContext *context);
    pm_region_offset (*get_root)(PmBackendContext *context);
    pm_region_offset (*malloc)(PmBackendContext *context, size_t size);
    void *(*read_object)(PmBackendContext *context, pm_region_offset offset);
    void (*write_object)(PmBackendContext *context, void *dst, char *data, size_t size);
};

int pm_init(PmWrapperConfig config);

pm_region_reference_id pm_init_reg(PmRegionConfig region_config);

void *pm_get_root_reg(pm_region_reference_id reference_id);
void *pm_get_root();

void *pm_alloc_reg(int size, pm_region_reference_id reference_id);
void *pm_alloc(int size);

void *pm_read_object(void *ptr);

void pm_write_object(void *ptr, char *data, int size);

void pm_close_reg(pm_region_reference_id reference_id);
void pm_close();

#define PM_R(ptr) ((typeof(ptr))pm_read_object((void *)(ptr)))

#define PM_W(field, value) (pm_write_object(&(field), (char *)(&(value)), sizeof(value)))

#define MAIN_REGION_REFERENCE_ID 1

#endif