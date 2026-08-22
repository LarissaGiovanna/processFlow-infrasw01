#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void run(Task *task)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); //cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        printf("id do filho: %d\n", getpid());
        int exe = execlp(task->program, task->args, NULL); //executa o programa
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
        }else{
            printf("programa %s executado com sucesso\n", task->program);
        }
        //exit(0); //encerra o processo filho
        
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        wait(NULL); //esperando o fillho terminar
        printf("Task %s concluida.\n", task->name);
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
       
    }
}

