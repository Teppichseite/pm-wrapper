#ifndef PM_WRAPPER_MULTIREGION_H
#define PM_WRAPPER_MULTIREGION_H

int pm_store_init(const char *path, const char *path1, const char *path2);
void pm_store_run();
void pm_store_close();

#endif