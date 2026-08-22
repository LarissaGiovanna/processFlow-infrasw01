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
        char input[256] = "";
        Task *head = NULL; // ponteiro para a primeira task da lista

        while (strcmp(input, "exit") != 0 || feof(stdin) == 0)
        {
            printf("processflow> ");
            fgets(input, sizeof(input), stdin);
            input[strcspn(input, "\n")] = '\0'; // remove \n do final da string

            printf("comando encontrado: %s\n", input);

            //========== se der tempo, jogar esse processo para uma funcao separada para ficar mais organizado ==========
            char *data[4] = {NULL, NULL, NULL, NULL}; // array para armazenar os dados do comando
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
                Task *taskToRun = findTask(head, taskName);
                if (taskToRun != NULL)
                {
                    printf("task a ser executada: %s\n", taskToRun->name);
                    run(taskToRun); //add a parte do run
                }
                else
                {
                    printf("Task %s nao encontrada.\n", taskName);
                }
            }
            else if (command && strcmp(command, "list") == 0)
            {
                printTasks(head); // imprime a lista de tasks
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

            if (strcmp(command, "exit") == 0 || feof(stdin) == 1)
            { // saida exit
                printf("Saindo do programa.\n");
                break;
            }
        }
    }
}