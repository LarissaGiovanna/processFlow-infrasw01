#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
//inicializacao modos interativo e workflow
if (argc > 1){ //workflow
    printf("%s\n", argv[1]);
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Arquivo nao encontrado. Verifique o caminho e tente novamente.\n");
        return 1;
    }else{
        char command[256];
        fgets(command, sizeof(command), file);
    
        //strtok para separar os comandos
        char *token;
        char *rest = command;
        while ((token = strtok_r(rest, " ", &rest))) {
            printf("comando encontrado: %s\n", token);
            //executar os comandos
        }
    fclose(file);
    return 0;
    }
}else{
    char command[256] = "";
    while (strcmp(command, "exit") != 0) {
        printf("processflow> ");
        scanf("%s", command);
        printf("comando encontrado: %s\n", command);

        if (strcmp(command, "exit") == 0 || feof(stdin)) { //saida exit
            printf("Saindo do programa.\n");
            break;
        }
    }

}

}


//processos
    // pid_t pid = fork();
    // if (pid == 0) {
    //     // Child process
    //     printf("Child process: %d\n", getpid());
    // } else if (pid > 0) {
    //     // Parent process
    //     printf("Parent process: %d\n", getpid());
    // } else {
    //     // Fork failed
    //     perror("fork");
    // }
