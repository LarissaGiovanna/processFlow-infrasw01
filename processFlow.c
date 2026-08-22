#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "task.h"

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
            
            Task *head = NULL; // ponteiro para a primeira task da lista
            if (command && strcmp(command, "task") == 0) 
            { // task
                if (data[1] == NULL)
                {
                    printf("Informe o nome da task.\n");
                    continue;
                }
                char *taskName = data[1]; // pega o nome da task
                printf("taskName: %s\n", taskName);

                
                
                if (taskName != "task" && taskName != NULL && strlen(taskName) > 0) //verificar para nao passar task duas vezes
                {
                    char *program = data[2]; // pega o nome do programa
                    char *args = data[3];    // pega os argumentos do programa
                    printf("Nome da task: %s\n", taskName);
                    head = createTask(head, taskName, program, args);
                }
                else
                {
                    printf("Nome da task nao fornecido.\n");
                }
            }

            if (strcmp(command, "exit") == 0 || feof(stdin) == 1)
            { // saida exit
                printf("Saindo do programa.\n");
                break;
            }
        }
    }
}

// processos
//  pid_t pid = fork();
//  if (pid == 0) {
//      // Child process
//      printf("Child process: %d\n", getpid());
//  } else if (pid > 0) {
//      // Parent process
//      printf("Parent process: %d\n", getpid());
//  } else {
//      // Fork failed
//      perror("fork");
//  }
