#ifndef PM_WRAPPER_H
#define PM_WRAPPER_H
#include <stdlib.h>

/**
 * Users can use all methods and macros that start with pm_ in their program.
 * PmBackend has to be implemented with a pm library and then passed to pm_init.
 *
 * There are two type of pointers:
 * - vm pointers: Normal virtual memory pointer which can be dereferenced
 * - pm pointers: Offset within the persistent memory region, can be dereferenced with PM_R
 *
 * So for example char *p can be either a vm char pointer or a pm char pointer.
 *
 * All methods (also the backend methods) use the POSIX return code standard.
 */

/**
 * @brief Offset in the memory region (explicit version of void*)
 */
typedef unsigned long pm_offset;

typedef struct PmWrapperConfig
{
    /**
     * @brief unique key of the program
     */
    char *key;
    /**
     * @brief file path for the pm region if the backend uses a file system
     */
    char *file_path;
    /**
     * @brief Size in bytes of the root object in the region
     */
    size_t root_size;
} PmWrapperConfig;

typedef struct PmBackend
{
    /**
     * @brief opens a pm region
     */
    int (*open)(PmWrapperConfig config);
    /**
     * @brief creates a pm region
     */
    int (*create)(PmWrapperConfig config);
    /**
     * @brief closes a pm region
     */
    int (*close)(PmWrapperConfig config);
    /**
     * @return pm offset of root in region
     */
    pm_offset (*get_root)();
    /**
     * @brief allocates size bytes in pm
     * @return start of allocated memory as pm offset
     */
    pm_offset (*malloc)(size_t size);
    /**
     * @brief Converts a pm offset into a vm pointer
     */
    void *(*read_object)(pm_offset offset);
    /**
     * @brief Writes data to a pm region
     * @param offset vm pointer
     * @param data vm pointer
     * @param size size of data to write
     */
    void (*write_object)(void* dst, char *data, size_t size);
} PmBackend;

/**
 * @brief Initializes a persistent memory region,
 * should be called at the beginning of the program
 */
int pm_init(PmWrapperConfig config, PmBackend backend);

/**
 * @brief Returns a pm pointer to the root object of the pm region
 */
void *pm_get_root();

/**
 * @brief Allocates memory in the pm region
 *
 * @param size size in bytes to allocate
 * @return void* pm pointer
 */
void *pm_alloc(int size);

/**
 * @brief Converts a pm pointer into vm pointer which can be dereferenced
 *
 * @param ptr pm pointer
 * @return void* vm pointer
 */
void *pm_read_object(void *ptr);

/**
 * @brief Writes data to pm
 *
 * @param ptr pm pointer
 * @param data vm pointer
 * @param size size of data to write
 */
void pm_write_object(void *ptr, char *data, int size);

/**
 * @brief Closes the pm region
 */
int pm_close();

/**
 * @brief Simplified and typesafe version of pm_read_object
 */
#define PM_R(ptr) ((typeof(ptr))pm_read_object((void *)(ptr)))

/**
 * @brief Simplified version of pm_write_object
 */
#define PM_W(object, value, size) (pm_write_object((object), (char *)(value), (size)))

#endif