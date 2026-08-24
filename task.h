typedef struct Task
{
    char *name;
    char *program;
    char *args;
    char *redirectFile;
    int redirectType; // 1 = input, 2 = output, 3 = append
    struct Task *next;
} Task;

Task *createTask(char *nameTask, char *program, char *args, char *redirectFile, int redirectType);
void addTask(Task** head, Task* newTask);
void printTasks(Task* head);
Task* findTask(Task* head, char* name);