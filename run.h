#include "task.h"

void run(Task *task);
void runSequential(Task* head, const char *argv[]);
void runParallel(Task* head, const char *argv[]);