#ifndef PMDK_MULTIREGION_H
#define PMDK_MULTIREGION_H

int pmdk_init(const char *path1, const char *path2);
void pmdk_run();
void pmdk_close();

#endif