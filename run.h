#include "task.h"
#include "job.h"

void run(Task *task);
void runSequential(Task* head, const char *argv[]);
void runParallel(Task* head, const char *argv[]);
void runPipe(Task* head, const char *argv[]);

void start(Task* task, Job** head, int *jobQnt);