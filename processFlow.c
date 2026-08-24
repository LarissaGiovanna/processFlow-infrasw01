#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"
#include "run.h"
#include "job.h"
#include <sys/types.h>
#if defined(__unix__) || defined(__APPLE__) // compatibilidade macOS
#include <sys/wait.h>
#endif

int main(int argc, char const *argv[])
{
    // inicializacao modos interativo e workflow
    if (argc > 1)
    { // workflow
        FILE *file = fopen(argv[1], "r");
        if (file == NULL)
        {
            fprintf(stderr, "Arquivo nao encontrado. Verifique o caminho e tente novamente.\n");
            return 1;
        }
        else
        {
            char line[512];
            Task *head = NULL; // ponteiro para a primeira task da lista
            Job *jobList = NULL; // ponteiro para a lista de jobs
            int jobQnt = 0;

            while (fgets(line, sizeof(line), file))
            {
                line[strcspn(line, "\r\n")] = '\0'; // remove \r e \n do final da string

                if (strlen(line) == 0)
                {
                    continue;
                }

                if (strcmp(line, "exit") == 0)
                {
                    break;
                }

                fprintf(stderr, "%s\n", line);

                char *data[512] = {NULL}; // array para armazenar os dados do comando
                char *token = strtok(line, " ");

                int i = 0;
                while (token)
                {
                    data[i] = token;
                    i++;
                    token = strtok(NULL, " ");
                }

                char *command = data[0]; // pega o nome do comando

                if (command && strcmp(command, "task") == 0)
                {
                    if (data[1] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task.\n");
                        continue;
                    }
                    char *taskName = data[1]; // pega o nome da task

                    if (taskName != NULL && strlen(taskName) > 0)
                    {
                        char *program = data[2]; // pega o nome do programa
                        char *args = data[3];    // pega os argumentos do programa
                        if (taskName != NULL && program != NULL)
                        {
                            addTask(&head, createTask(taskName, program, args, NULL, 0));
                        }
                        else
                        {
                            fprintf(stderr, "Nome da task ou programa nao fornecido.\n");
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Nome da task nao fornecido.\n");
                    }
                }
                else if (command && strcmp(command, "run") == 0)
                {
                    if (data[1] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task a ser executada.\n");
                        continue;
                    }
                    char *taskName = data[1]; // pega o nome da task

                    if (strcmp(taskName, "sequential") == 0)
                    {
                        const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                        for (int i = 2; data[i] != NULL; i++)
                        {
                            taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                        }
                        runSequential(head, taskList); // executa os comandos sequenciais
                    }
                    else if (strcmp(taskName, "parallel") == 0)
                    {
                        const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                        for (int i = 2; data[i] != NULL; i++)
                        {
                            taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                        }
                        runParallel(head, taskList); // executa os comandos em paralelo
                    }
                    else if (strcmp(taskName, "pipe") == 0)
                    {
                        const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                        for (int i = 2; data[i] != NULL; i++)
                        {
                            taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                        }
                        runPipe(head, taskList); // executa os comandos em pipe
                    }
                    else
                    {
                        Task *taskToRun = findTask(head, taskName);
                        if (taskToRun == NULL)
                        {
                            fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                        }
                    }
                }
                else if (command && strcmp(command, "list") == 0)
                {
                    printTasks(head); // imprime a lista de tasks
                }
                else if (command && strcmp(command, "workdir") == 0)
                {
                    char* directory = data[1];
                    if (directory != NULL && strlen(directory) > 0)
                    {
                        int result = chdir(directory);
                        if (result == 0)
                        {
                            fprintf(stderr, "Diretorio de trabalho alterado para: %s\n", getcwd(NULL, 0));
                        }
                        else
                        {
                            perror("Erro ao alterar o diretorio de trabalho");
                        }
                    }
                    else
                    {
                        fprintf(stderr, "Informe o caminho do diretorio.\n");
                    }
                }
                else if (command && strcmp(command, "input") == 0)
                {
                    if (data[1] == NULL || data[2] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task e o arquivo de entrada.\n");
                        continue;
                    }
                    char *taskName = data[1];
                    char *inputFile = data[2];

                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun != NULL)
                    {
                        taskToRun->redirectFile = strdup(inputFile);
                        taskToRun->redirectType = 1; // 1 = input
                    }
                    else
                    {
                        fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                    }
                }
                else if (command && strcmp(command, "output") == 0)
                {
                    if (data[1] == NULL || data[2] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task e o arquivo de saida.\n");
                        continue;
                    }
                    char *taskName = data[1];
                    char *outputFile = data[2];

                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun != NULL)
                    {
                        taskToRun->redirectFile = strdup(outputFile);
                        taskToRun->redirectType = 2; // 2 = output
                    }
                    else
                    {
                        fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                    }
                }
                else if (command && strcmp(command, "append") == 0)
                {
                    if (data[1] == NULL || data[2] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task e o arquivo de saida.\n");
                        continue;
                    }
                    char *taskName = data[1];
                    char *appendFile = data[2];

                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun != NULL)
                    {
                        taskToRun->redirectFile = strdup(appendFile);
                        taskToRun->redirectType = 3; // 3 = append
                    }
                    else
                    {
                        fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                    }
                }
                else if (command && strcmp(command, "start") == 0)
                {
                    if (data[1] == NULL)
                    {
                        fprintf(stderr, "Informe o nome da task a ser iniciada.\n");
                        continue;
                    }
                    char *taskName = data[1];

                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun != NULL)
                    {
                        start(taskToRun, &jobList, &jobQnt);
                    }
                    else
                    {
                        fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                    }
                }
                else if (command && strcmp(command, "jobs") == 0)
                {
                    printJobs(&jobList);
                }
                else if (command && strcmp(command, "wait") == 0)
                {
                    if (data[1] == NULL)
                    {
                        fprintf(stderr, "Informe o ID do job a ser aguardado.\n");
                        continue;
                    }
                    int jobId = atoi(data[1]);

                    Job* jobToWait = findJob(jobList, jobId);
                    if (jobToWait != NULL)
                    {
                        waitpid(jobToWait->pid, NULL, 0);
                        fprintf(stderr, "Job %d concluido.\n", jobToWait->job_id);
                        removeJob(&jobList, jobToWait->job_id);
                    }
                    else
                    {
                        fprintf(stderr, "Job %d nao encontrado.\n", jobId);
                    }
                }
                else if (command && strlen(command) > 0)
                {
                    fprintf(stderr, "Comando desconhecido: %s\n", command);
                }
            }
            fclose(file);
            return 0;
        }
    }
    else
    { // interativo
        char input[512] = "";
        Task *head = NULL; // ponteiro para a primeira task da lista
        Job* jobList = NULL; //ponteiro para a primeira task da lista de jobs
        
        while (strcmp(input, "exit") != 0 || feof(stdin) == 0)
        {
            fprintf(stderr, "processflow> ");
            fflush(stdout); //limpa o buffer de saida para que o prompt seja exibido imediatamente
            fgets(input, sizeof(input), stdin);

            input[strcspn(input, "\n")] = '\0'; // remove \n do final da string

            if (strcmp(input, "exit") == 0 || feof(stdin) != 0)
            { // saida exit
                fprintf(stderr, "Saindo do programa.\n");
                break;
            }

            //========== se der tempo, jogar esse processo para uma funcao separada para ficar mais organizado ==========
            char *data[512] = {NULL}; // array para armazenar os dados do comando
            char *token = strtok(input, " ");

            int i = 0;
            while (token)
            {
                data[i] = token;
                i++;
                token = strtok(NULL, " ");
            }
            //==== ate aqui =====
            int jobQnt = 0;
            char *command = data[0]; // pega o nome do comando

            if (command && strcmp(command, "task") == 0)
            {
                if (data[1] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task

                if (taskName != "task" && taskName != NULL && strlen(taskName) > 0) // verificar para nao passar task duas vezes
                {
                    char *program = data[2]; // pega o nome do programa
                    char *args = data[3];    // pega os argumentos do programa
                    if (taskName != NULL && program != NULL)
                    {
                        addTask(&head, createTask(taskName, program, args, NULL, 0));
                        printTasks(head); // imprime a lista de tasks
                    }
                    else
                    { // segmentation fault se nao passar o args
                        fprintf(stderr, "Nome da task ou programa nao fornecido.\n");
                    }
                }
                else
                {
                    fprintf(stderr, "Nome da task nao fornecido.\n");
                }
            }
            else if (command && strcmp(command, "run") == 0)
            {
                if (data[1] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task a ser executada.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task

                if (strcmp(taskName, "sequential") == 0)
                {
                    const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                    for (int i = 2; data[i] != NULL; i++)
                    {
                        taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                    }
                    runSequential(head, taskList); // executa os comandos sequenciais
                }
                else if (strcmp(taskName, "parallel") == 0)
                {
                    const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                    for (int i = 2; data[i] != NULL; i++)
                    {
                        taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                    }
                    runParallel(head, taskList); // executa os comandos em paralelo
                }
                else if (strcmp(taskName, "pipe") == 0)
                {
                    const char* taskList[400] = {NULL}; // array para armazenar os nomes das tasks
                    for (int i = 2; data[i] != NULL; i++)
                    {
                        taskList[i - 2] = data[i]; // armazena os nomes das tasks no array
                    }
                    runPipe(head, taskList); // executa os comandos em pipe
                }
                else
                {
                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun == NULL)
                    {
                        fprintf(stderr, "Task %s nao encontrada.\n", taskName);
                    }
                }
            }
            else if (command && strcmp(command, "list") == 0)
            {
                printTasks(head); // imprime a lista de tasks
            }
            else if (command && strcmp(command, "workdir") == 0){
                char* directory = data[1];
                if (directory != NULL && strlen(directory) > 0){
                    int result = chdir(directory);
                    if (result == 0){
                        fprintf(stderr, "Diretorio de trabalho alterado para: %s\n", getcwd(NULL, 0));
                    } else {
                        perror("Erro ao alterar o diretorio de trabalho");
                    }
                } else {
                    fprintf(stderr, "Informe o caminho do diretorio.\n");
                }
            }
            else if (command && strcmp(command, "input") == 0)
            {
                if (data[1] == NULL || data[2] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task e o arquivo de entrada.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task
                char *inputFile = data[2]; // pega o arquivo de entrada

                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    taskToRun->redirectFile = strdup(inputFile);
                    taskToRun->redirectType = 1; // 1 = inputun);
                }
                else
                {
                    fprintf(stderr,"Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "output") == 0)
            {
                if (data[1] == NULL || data[2] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task e o arquivo de saida.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task
                char *outputFile = data[2]; // pega o arquivo de saida

                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    taskToRun->redirectFile = strdup(outputFile);
                    taskToRun->redirectType = 2; // 2 = output
                }
                else
                {
                    fprintf(stderr,"Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "append") == 0){
                if (data[1] == NULL || data[2] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task e o arquivo de saida.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task
                char *appendFile = data[2]; // pega o arquivo de saida

                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    taskToRun->redirectFile = strdup(appendFile);
                    taskToRun->redirectType = 3; // 3 = append
                }
                else
                {
                    fprintf(stderr,"Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "start") == 0)
            {
                if (data[1] == NULL)
                {
                    fprintf(stderr, "Informe o nome da task a ser iniciada.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task

                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    start(taskToRun, &jobList, &jobQnt);
                }
                else
                {
                    fprintf(stderr,"Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "jobs") == 0)
            {
                printJobs(&jobList);
            }
            else if (command && strcmp(command, "wait") == 0){
                if (data[1] == NULL)
                {
                    fprintf(stderr, "Informe o ID do job a ser aguardado.\n");
                    continue;
                }
                int jobId = atoi(data[1]); // atoi converte string para int

                Job* jobToWait = findJob(jobList, jobId);
                if (jobToWait != NULL)
                {
                    fprintf(stderr, "Aguardando o job %d...\n", jobToWait->job_id);
                    waitpid(jobToWait->pid, NULL, 0); // aguarda o término do processo
                    fprintf(stderr, "Job %d concluido.\n", jobToWait->job_id);
                    removeJob(&jobList, jobToWait->job_id); // remove o job da lista
                }
                else
                {
                    fprintf(stderr, "Job %d nao encontrado.\n", jobId);
                }
            }
            else if (command && strlen(command) > 0)
            {
                fprintf(stderr, "Comando desconhecido: %s\n", command);
            }
        }
    }
}