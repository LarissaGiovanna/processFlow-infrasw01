#include "task.h"
#include "job.h"

void run(Task *task);
void runSequential(Task* head, const char *argv[]);
void runParallel(Task* head, const char *argv[]);
void runPipe(Task* head, const char *argv[]);

void redirectInput(Task *task, char* inputFile);
void redirectOutput(Task *task, char* outputFile);
void redirectAppend(Task *task, char* appendFile);

void start(Task* task, Job** head, int *jobQnt);