#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"
#include "run.h"

int main(int argc, char const *argv[])
{
    // inicializacao modos interativo e workflow
    if (argc > 1)
    { // workflow
        printf("%s\n", argv[1]);
        FILE *file = fopen(argv[1], "r");
        if (file == NULL)
        {
            printf("Arquivo nao encontrado. Verifique o caminho e tente novamente.\n");
            return 1;
        }
        else
        {
            char command[256];
            fgets(command, sizeof(command), file);

            // strtok para separar os comandos
            char *token;
            char *rest = command;
            while ((token = strtok_r(rest, " ", &rest)))
            {
                printf("comando encontrado: %s\n", token);
                // executar os comandos
            }
            fclose(file);
            return 0;
        }
    }
    else
    { // interativo
        char input[512] = "";
        Task *head = NULL; // ponteiro para a primeira task da lista

        while (strcmp(input, "exit") != 0 || feof(stdin) == 0)
        {
            printf("processflow> ");
            fgets(input, sizeof(input), stdin);

            input[strcspn(input, "\n")] = '\0'; // remove \n do final da string

            if (strcmp(input, "exit") == 0 || feof(stdin) != 0)
            { // saida exit
                printf("Saindo do programa.\n");
                break;
            }

            printf("comando encontrado: %s\n", input);

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

            char *command = data[0]; // pega o nome do comando

            if (command && strcmp(command, "task") == 0)
            {
                if (data[1] == NULL)
                {
                    printf("Informe o nome da task.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task

                if (taskName != "task" && taskName != NULL && strlen(taskName) > 0) // verificar para nao passar task duas vezes
                {
                    char *program = data[2]; // pega o nome do programa
                    char *args = data[3];    // pega os argumentos do programa
                    printf("Nome da task: %s\n", taskName);
                    printf("Programa: %s\n", program);
                    printf("args: %s\n", args);
                    if (taskName != NULL && program != NULL)
                    {
                        addTask(&head, createTask(taskName, program, args));
                        printf("Task %s adicionada com sucesso.\n", taskName);
                        printTasks(head); // imprime a lista de tasks
                    }
                    else
                    { // segmentation fault se nao passar o args
                        printf("Nome da task ou programa nao fornecido.\n");
                    }
                }
                else
                {
                    printf("Nome da task nao fornecido.\n");
                }
            }
            else if (command && strcmp(command, "run") == 0)
            {
                if (data[1] == NULL)
                {
                    printf("Informe o nome da task a ser executada.\n");
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
                else
                {
                    Task *taskToRun = findTask(head, taskName);
                    if (taskToRun != NULL)
                    {
                        printf("task a ser executada: %s\n", taskToRun->name);
                        run(taskToRun);
                    }
                    else
                    {
                        printf("Task %s nao encontrada.\n", taskName);
                    }
                }
            }
            else if (command && strcmp(command, "list") == 0)
            {
                printTasks(head); // imprime a lista de tasks
            }
            else if (command && strcmp(command, "workdir") == 0){
                printf("diretorio atual: %s\n", getcwd(NULL, 0));
                char* directory = data[1];
                if (directory != NULL && strlen(directory) > 0){
                    int result = chdir(directory);
                    if (result == 0){
                        printf("Diretorio de trabalho alterado para: %s\n", getcwd(NULL, 0));
                    } else {
                        perror("Erro ao alterar o diretorio de trabalho");
                    }
                } else {
                    printf("Informe o caminho do diretorio.\n");
                }
            }
            else if (command && strcmp(command, "input") == 0)
            {
                if (data[1] == NULL || data[2] == NULL)
                {
                    printf("Informe o nome da task e o arquivo de entrada.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task
                char *inputFile = data[2]; // pega o arquivo de entrada

                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    printf("task a ser executada: %s\n", taskToRun->name);
                    redirectInput(taskToRun, inputFile);
                    printf("a entrada do arquivo %s foi redirecionada para a task %s\n", inputFile, taskToRun->name);
                }
                else
                {
                    printf("Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "help") == 0)
            {
                printf("Comandos disponiveis:\n");
                printf("task <nome> <programa> <args> - Adiciona uma nova task\n");
                printf("run <nome> - Executa a task especificada\n");
                printf("list - Lista todas as tasks\n");
                printf("exit - Sai do programa\n");
            }
            else if (command && strlen(command) > 0)
            {
                printf("Comando desconhecido: %s\n", command);
            }
        }
    }
}