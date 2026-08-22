# diario
- [20/08 - 17:54]comparar entrada do teclado (fgets) com string (char *) com strcmp deu resultados estranhos e nao esperados. na vdd, a string digitada pelo usuario vem com \n no final, interferindo na comparação final. usei strstr para verficar se a palavra "task" esta contida na string passada e verifiquei se o endereço de task é igual ao endereço que é retornado pela funcao strstr
- [21/08 - 21:26] tentar separar palavras vindas do comando para variaveis separadas ta dando errado com strtok pq nao sei usar direito. [21:39 ] deu certo usando while 
- [21/08 - 21:26] consegui separar as strings usadno strtok e nao precisou da comparação que eu fiz ontem usando strstr
- [21/08 - 22:02] DECISAO: usar lista encadeada para guardar as tarefas cadastradas
- [21/08 - 23:08] funcoes de lista encadeada nao estavam funcionando e tavam dando segmentation fault, mas descobri que era pq tava pasando o head errado e depois de corrigir funcionou todas as outras funcoes


# por quesw
- [21/08 - 21:50]ao dar ctrl + d quando inicia o programa, da segmetantion fault e o programa esta se encerrando por conta disso, nao por conta do ctrl d
- [21/08 - 22:33] ao nao passar o agumento q eh opcional em task, da segmetation fault, mesmo sabedno que a variavel argumento é NULL por padrao
- [21/08 - 22:37] ao add uma tarefa e tentar sair com ctrl d, fica em looping e depois para esperando alguma entrada
- [21/08 - 22:40] make: 'processFlow' is up to date. [OK]
- [21/08 - 22:42] print de task.c na funcao de printar as tasks nao ta funcionando. funcionou e era pq o head nao estava sendo passado corretamente
- [21/08 - 22:56] task nao ta sendo adicoinado na lista e com strcpy na funcao de adicionar deu segmentation fault, e com strdup nao ta sendo adicionado