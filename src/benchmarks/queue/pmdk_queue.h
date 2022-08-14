
#ifndef QUEUE_PMDK_H
#define QUEUE_PMDK_H

int pmdk_init(const char *path);
void pmdk_run();
void pmdk_close();

#endif