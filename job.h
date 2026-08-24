typedef struct job{
    int job_id;
    int pid;
    int isActive;
    struct job *next;
} Job;

Job* createJob(int job_id, int pid);
void addJob(Job** head, int job_id, int pid);
void printJobs(Job* head);
Job* findJob(Job* head, int job_id);
void removeJob(Job** head, int job_id);