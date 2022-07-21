#ifndef PMDK_STORE_H
#define PMDK_STORE_H

int pmdk_init(const char* path);
void pmdk_run();
void pmdk_close();

#endif