# include "job.h"
# include <stdio.h>
# include <stdlib.h>

Job* createJob(int job_id, int pid){
    Job* newJob = (Job*)malloc(sizeof(Job));
    if(newJob == NULL){
        printf("erro na hora da alocacao de memoria para o novo job\n");
        return NULL;
    }
    newJob->job_id = job_id;
    newJob->pid = pid;
    newJob->isActive = 1;
    newJob->next = NULL;
    return newJob;
}

void addJob(Job** head, Job* newJob){
    if(newJob == NULL){
        printf("erro na hora de criar o novo job\n");
        return;
    }
    if(*head == NULL){
        *head = newJob;
    }else{
        sortJobs(head);
        Job* atual = *head;
        while(atual->next != NULL){
            atual = atual->next;
        }
        atual->next = newJob;
    }
}

void printJobs(Job** head){
    if (*head == NULL) {
        printf("Nenhum job em execucao.\n");
        return;
    }
    Job* atual = *head;
    while(atual != NULL){
        printf("Job ID: %d, PID: %d\n", atual->job_id, atual->pid);
        atual = atual->next;
    }
}

Job* findJob(Job* head, int job_id){
    Job* atual = head;
    while(atual != NULL){
        if(atual->job_id == job_id){
            return atual;
        }
        atual = atual->next;
    }
    return NULL;
}

void removeJob(Job** head, int job_id){
    if (*head == NULL) {
        return;
    }
    if ((*head)->job_id == job_id) {
        Job* temp = *head;
        *head = (*head)->next;
        free(temp);
        return;
    }
    Job* atual = *head;
    while (atual->next != NULL && atual->next->job_id != job_id) {
        atual = atual->next;
    }
    if (atual->next != NULL) {
        Job* temp = atual->next;
        atual->next = atual->next->next;
        free(temp);
    }
}

void sortJobs(Job** head){
    if (*head == NULL) {
        return;
    }
    Job* atual = *head;
    while (atual->next != NULL) {
        if (atual->job_id > atual->next->job_id) {
            // troca os jobs
            int temp = atual->job_id;
            atual->job_id = atual->next->job_id;
            atual->next->job_id = temp;
        }
        atual = atual->next;
    }
}