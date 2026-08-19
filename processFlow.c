#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
//inicializacao modos interativo e workflow
if (argc > 1){
    printf("%s\n", argv[1]);
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Arquivo nao encontrado. Verifique o caminho e tente novamente.\n");
        return 1;
    }else{
        char command[256];
        fgets(command, sizeof(command), file);
        printf("comandos: %s\n", command);
    
        //strtok para separar os comandos
        char *token;
        char *rest = command;
        while ((token = strtok_r(rest, " ", &rest))) {
            printf("Executando comando: %s\n", token);
        }
    fclose(file);
    return 0;
}}
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
