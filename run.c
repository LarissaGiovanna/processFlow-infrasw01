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
        fprintf(stderr,"Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho
        if (task->redirectFile != NULL)
        {
            int fileDescriptor;
            if (task->redirectType == 1) // input
            {
                fileDescriptor = open(task->redirectFile, O_RDONLY);
                if (fileDescriptor < 0)
                {
                    fprintf(stderr, "Erro ao abrir o arquivo de entrada %s\n", task->redirectFile);
                    exit(1);
                }
                dup2(fileDescriptor, STDIN_FILENO);
            }
            else if (task->redirectType == 2) // output
            {
                fileDescriptor = open(task->redirectFile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fileDescriptor < 0)
                {
                    fprintf(stderr, "Erro ao abrir o arquivo de saida %s\n", task->redirectFile);
                    exit(1);
                }
                dup2(fileDescriptor, STDOUT_FILENO);
            }
            else if (task->redirectType == 3) // append
            {
                fileDescriptor = open(task->redirectFile, O_WRONLY | O_CREAT | O_APPEND, 0644);
                if (fileDescriptor < 0)
                {
                    fprintf(stderr, "Erro ao abrir o arquivo de saida %s\n", task->redirectFile);
                    exit(1);
                }
                dup2(fileDescriptor, STDOUT_FILENO);
            }
        }
        int exe = execlp(task->program, task->program, task->args, NULL); // executa o programa
        if (exe == -1)
        {
            fprintf(stderr, "Erro ao executar o programa %s\n", task->program);
        }
    }
    else if (pid > 0)
    {
        // pai
        wait(NULL); // esperando o fillho terminar
    }
    else
    {
        fprintf(stderr, "Erro ao executar a task %s\n", task->name);
    }
}

void runSequential(Task *head, const char *argv[])
{
    if (argv == NULL)
    {
        fprintf(stderr, "Nenhuma tarefa fornecida\n");
        return;
    }

    for (int i = 0; argv[i] != NULL; i++) // percorrer todos os args
    {
        run(findTask(head, (char *)argv[i])); // procura a task na lista de tasks e executa
    }
}
void runParallel(Task *head, const char *argv[])
{
    if (argv == NULL)
    {
        fprintf(stderr, "Nenhuma tarefa fornecida\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo
    if (pid == 0)
    {
        // filho
        for (int i = 0; argv[i] != NULL; i++) // percorrer todos os args
        {
            pid_t pidTask = fork();                       // cria um novo processo para cada comando
            Task *task = findTask(head, (char *)argv[i]); // procura a task na lista de tasks

            if (pidTask == 0)
            {
                // filho
                int exe = execlp(task->program, task->program, task->args, NULL); // executa o programa
                if (exe == -1)
                {
                    fprintf(stderr, "Erro ao executar o comando %s\n", argv[i]);
                }
            }
        }
        exit(0);
    }
    else if (pid > 0)
    {
        // pai
        wait(NULL); // esperando o fillho terminar
    }
    else
    {
        fprintf(stderr, "Erro ao executar os comandos \n");
    }
}

void start(Task *task, Job **head, int *jobQnt)
{
    if (task == NULL)
    {
        fprintf(stderr, "Task nao encontrada.\n");
        return;
    }

    pid_t pid = fork(); // cria um novo processo

    if (pid == 0)
    {
        // filho

        int exe = execlp(task->program, task->program, task->args, NULL); // executa o programa

        if (exe == -1)
        {
            fprintf(stderr, "Erro ao executar o programa %s\n", task->program);
            exit(1);
        }
    }
    else if (pid > 0)
    {
        // pai
        (*jobQnt)++;
        addJob(head, createJob(*jobQnt, pid)); // adiciona o novo job a lista de jobs
        fprintf(stderr, "[%d] %d\n", *jobQnt, pid);
        printJobs(head); // imprime a lista de jobs
        fflush(stdout);  // limpa o buffer de saida para que o prompt seja exibido imediatamente
        // wait(NULL); // esperando o fillho terminar
    }
    else
    {
        fprintf(stderr, "Erro ao executar a task %s\n", task->name);
    }
}

void runPipe(Task *head, const char *argv[])
{
    if (argv == NULL)
    {
        fprintf(stderr, "Nenhuma tarefa fornecida\n");
        return;
    }
    int taskCount = 0;

    for (int i = 0; argv[i] != NULL; i++) // percorrer todos os args e contar quantas tarefas tem
    {
        taskCount++;
    }

    int pipefd[2 * (taskCount - 1)]; // array para armazenar os pipes

    for (int i = 0; i < taskCount - 1; i++)
    {
        if (i < taskCount - 1) // se nao for a ultima tarefa, cria um pipe
        {
            // cria um pipe
            if (pipe(&pipefd[2 * i]) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }
    }

    for (int i = 0; i < taskCount; i++)
    {

        Task *task = findTask(head, (char *)argv[i]); // procura a task na lista de tasks
        pid_t pid = fork();                           // cria um novo processo

        if (pid == 0)
        {
            // filho
            if (i > 0) // entrada para o pipe
            {
                dup2(pipefd[2 * (i-1)], STDIN_FILENO);
                // close(pipefd[0]);
            }
            if (i < taskCount - 1) // saida para o pipe
            {
                dup2(pipefd[2 * i + 1], STDOUT_FILENO);
                // close(pipefd[1]);
            }

            for (int j = 0; j < 2 * (taskCount - 1); j++) // fecha os pipes
            {
                close(pipefd[j]);
            }

            int exe = execlp(task->program, task->program, task->args, NULL); // executa o programa
            if (exe == -1)
            {
                fprintf(stderr, "Erro ao executar o comando %s\n", argv[i]);
                exit(1);
            }
        }
        else if (pid > 0)
        {
            // pai
            // wait(NULL); // esperando o fillho terminar

        }
        else
        {
            fprintf(stderr, "Erro ao executar o comando %s\n", argv[i]);
        }
    }
    for (int i = 0; i < 2 * (taskCount - 1); i++) // fecha os pipes
    {
        close(pipefd[i]);
    }
    for (int i = 0; i < taskCount; i++) // espera todos os filhos terminarem
    {
        wait(NULL);
    }
}
