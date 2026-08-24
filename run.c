#include "run.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#if defined(__unix__) || defined(__APPLE__) // compatibilidade macOS
#include <sys/wait.h>
#endif
#include <fcntl.h>
#include <string.h>
#include "job.h"

void run(Task *task)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        printf("id do filho: %d\n", getpid());
        int exe = execlp(task->program, task->args, NULL); // executa o programa
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
        }
        else
        {
            printf("programa %s executado com sucesso\n", task->program);
        }
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        wait(NULL); // esperando o fillho terminar
        printf("Task %s concluida.\n", task->name);
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
    }
}

void runSequential(Task *head, const char *argv[])
{
    if (argv == NULL)
    {
        printf("nenhuma tarefa fornecida\n");
        return;
    }

    for (int i = 0; argv[i] != NULL; i++) // percorrer todos os args
    {
        Task *task = findTask(head, (char *)argv[i]); // procura a task na lista de tasks
        pid_t pid = fork();                           // cria um novo processo

        if (pid == 0)
        {
            // filho
            printf("Executando comando: %s\n", argv[i]);
            int exe = execlp(task->program, task->args, NULL); // executa o programa
            if (exe == -1)
            {
                printf("Erro ao executar o comando %s\n", argv[i]);
            }
            exit(0);
        }
        else if (pid > 0)
        {
            // pai
            wait(NULL); // esperando o fillho terminar
            printf("Comando %s concluido.\n", argv[i]);
        }
        else
        {
            printf("Erro ao executar o comando %s\n", argv[i]);
        }
    }
}
void runParallel(Task *head, const char *argv[])
{
    if (argv == NULL)
    {
        printf("nenhuma tarefa fornecida\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo
    if (pid == 0)
    {
        // filho
        printf("Executando comandos em paralelo\n");
        for (int i = 0; argv[i] != NULL; i++) // percorrer todos os args
        {
            pid_t pidTask = fork();                       // cria um novo processo para cada comando
            Task *task = findTask(head, (char *)argv[i]); // procura a task na lista de tasks

            if (pidTask == 0)
            {
                // filho
                printf("Executando comando: %s\n", argv[i]);
                int exe = execlp(task->program, task->args, NULL); // executa o programa
                if (exe == -1)
                {
                    printf("Erro ao executar o comando %s\n", argv[i]);
                }
                exit(0);
            }
        }
        exit(0);
    }
    else if (pid > 0)
    {
        // pai
        printf("Comandos enviados para serem executados em paralelo\n");
        wait(NULL); // esperando o fillho terminar
        printf("terminou");
    }
    else
    {
        printf("Erro ao executar os comandos \n");
    }

    printf("Todos os comandos foram executados em paralelo\n");
}

void redirectInput(Task *task, char *inputFile)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        printf("id do filho: %d\n", getpid());

        inputFile[strcspn(inputFile, "\n")] = '\0';
        int fileDescriptor = open(inputFile, O_RDONLY); // abre o arquivo de entrada
        if (fileDescriptor < 0)
        {
            printf("Erro ao abrir o arquivo de entrada\n");
            exit(1);
        }
        printf("arquivo de entrada %s aberto com sucesso\n", inputFile);
        dup2(fileDescriptor, STDIN_FILENO);                // redireciona a entrada padrão para o arquivo
        close(fileDescriptor);                             // fecha o arquivo de entrada
        int exe = execlp(task->program, task->args, NULL); // executa o programa
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
            exit(1);
        }
        else
        {
            printf("programa %s executado com sucesso\n", task->program);
            exit(0);
        }
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        wait(NULL); // esperando o fillho terminar
        printf("Task %s concluida.\n", task->name);
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
    }
}
void redirectOutput(Task *task, char *outputFile)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        printf("id do filho: %d\n", getpid());

        outputFile[strcspn(outputFile, "\n")] = '\0';
        int fileDescriptor = open(outputFile, O_WRONLY | O_TRUNC | O_CREAT, 0644); // abre o arquivo de entrada
        if (fileDescriptor < 0)
        {
            printf("Erro ao abrir o arquivo de entrada\n");
            exit(1);
        }
        printf("arquivo de entrada %s aberto com sucesso\n", outputFile);
        dup2(fileDescriptor, STDOUT_FILENO);               // redireciona a saída padrão para o arquivo
        close(fileDescriptor);                             // fecha o arquivo de entrada
        int exe = execlp(task->program, task->args, NULL); // executa o programa
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
            exit(1);
        }
        else
        {
            printf("programa %s executado com sucesso\n", task->program);
            exit(0);
        }
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        wait(NULL); // esperando o fillho terminar
        printf("Task %s concluida.\n", task->name);
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
    }
}
void redirectAppend(Task *task, char *appendFile)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        printf("id do filho: %d\n", getpid());

        appendFile[strcspn(appendFile, "\n")] = '\0';
        int fileDescriptor = open(appendFile, O_APPEND | O_WRONLY); // abre o arquivo de entrada
        if (fileDescriptor < 0)
        {
            printf("Erro ao abrir o arquivo de entrada\n");
            exit(1);
        }
        printf("arquivo de entrada %s aberto com sucesso\n", appendFile);
        dup2(fileDescriptor, STDOUT_FILENO);               // redireciona a saída padrão para o arquivo
        close(fileDescriptor);                             // fecha o arquivo de entrada
        int exe = execlp(task->program, task->args, NULL); // executa o programa
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
            exit(1);
        }
        else
        {
            printf("programa %s executado com sucesso\n", task->program);
            exit(0);
        }
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        wait(NULL); // esperando o fillho terminar
        printf("Task %s concluida.\n", task->name);
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
    }
}

void start(Task *task, Job **head, int *jobQnt)
{
    if (task == NULL)
    {
        printf("Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        printf("Executando task: %s\n", task->name);
        fflush(stdout);
        
        int exe = execlp(task->program, task->args, NULL); // executa o programa
        
        if (exe == -1)
        {
            printf("Erro ao executar o programa %s\n", task->program);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        // pai
        printf("id do pai: %d\n", getpid());
        (*jobQnt)++;
        addJob(head, createJob(*jobQnt, pid)); // adiciona o novo job a lista de jobs
        printf("[%d] %d\n", *jobQnt, pid);
        printJobs(head); // imprime a lista de jobs
        fflush(stdout); //limpa o buffer de saida para que o prompt seja exibido imediatamente
        // wait(NULL); // esperando o fillho terminar
        
    }
    else
    {
        printf("Erro ao executar a task %s\n", task->name);
    }
}
