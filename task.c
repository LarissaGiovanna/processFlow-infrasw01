#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"

Task *createTask(char *nameTask, char *program, char *args){
    Task *newTask = (Task *)malloc(sizeof(Task));
    newTask->name = strdup(nameTask); //strdup aloca memoria e copia a string para o novo ponteiro
    newTask->program = strdup(program);
    newTask->args = strdup(args);
    newTask->next = NULL;
    return newTask;
}

void addTask(Task* head, Task* newTask){
    if (head == NULL) {
        head = newTask;
    } else {
        Task *atual = head;
        while (atual->next != NULL) {
            atual = atual->next;
        }
        atual->next = newTask;
    }
}

void printTasks(Task* head) {
    Task *atual = head;
    while (atual != NULL) {
        printf("Task Name: %s, Program: %s, Args: %s\n", atual->name, atual->program, atual->args);
        atual = atual->next;
    }
}

Task* findTask(Task* head, char* name) {
    Task *atual = head;
    while (atual != NULL) {
        if (strcmp(atual->name, name) == 0) {
            return atual;
        }
        atual = atual->next;
    }
    return NULL; // Retorna null se a task não for encontrada
}