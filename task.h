typedef struct Task
{
    char *name;
    char *program;
    char *args;
    struct Task *next;
} Task;

Task *createTask(char *nameTask, char *program, char *args);
void addTask(Task** head, Task* newTask);
void printTasks(Task* head);
Task* findTask(Task* head, char* name);