# diario
- [20/08 - 17:54]comparar entrada do teclado (fgets) com string (char *) com strcmp deu resultados estranhos e nao esperados. na vdd, a string digitada pelo usuario vem com \n no final, interferindo na comparação final. usei strstr para verficar se a palavra "task" esta contida na string passada e verifiquei se o endereço de task é igual ao endereço que é retornado pela funcao strstr
- [21/08 - 21:26] tentar separar palavras vindas do comando para variaveis separadas ta dando errado com strtok pq nao sei usar direito. [21:39 ] deu certo usando while 
- [21/08 - 21:26] consegui separar as strings usadno strtok e nao precisou da comparação que eu fiz ontem usando strstr
- [21/08 - 22:02] DECISAO: usar lista encadeada para guardar as tarefas cadastradas
- [21/08 - 23:08] funcoes de lista encadeada nao estavam funcionando e tavam dando segmentation fault, mas descobri que era pq tava pasando o head errado e depois de corrigir funcionou todas as outras funcoes
- [22/08 - 14:18] ao executar uma tarefa e o programa nao existir, ele diz que nao conseguiu e quando é apertado ctrl d para sair do programa, ele encerra o processo que deu errado e depois fica esperando alguma entrada
- [23:08 - 16:34] corrigido args qunado nao tinha. era so fazer um if e colocar ele como null na hora de criar uma task
- [23/08 - 16:49] programa so funciona se tiver a / no começo do caminho (fazer uma funcao para colocar a barra no começo)


# por quesw
- [21/08 - 21:50]ao dar ctrl + d quando inicia o programa, da segmetantion fault e o programa esta se encerrando por conta disso, nao por conta do ctrl d [22/08 - 13:08] corrigido com a condicional verificando o feof(stdin) sendo passada para ser a primeira verficacao do while 
- [21/08 - 22:33] ao nao passar o agumento q eh opcional em task, da segmetation fault, mesmo sabedno que a variavel argumento é NULL por padrao
- [21/08 - 22:37] ao add uma tarefa e tentar sair com ctrl d, fica em looping e depois para esperando alguma entrada
- [21/08 - 22:40] make: 'processFlow' is up to date. [OK]
- [21/08 - 22:42] print de task.c na funcao de printar as tasks nao ta funcionando. funcionou e era pq o head nao estava sendo passado corretamente
- [21/08 - 22:56] task nao ta sendo adicoinado na lista e com strcpy na funcao de adicionar deu segmentation fault, e com strdup nao ta sendo adicionado
- [22/08 - 15:17] ao executar a funcao de run parallel a funcao executa corretamente mas o print do process flow nao aparece apos o termino da funcao, mas o programa esta esperando a entrada do fgets
- [23/08 - 17:57] ao fazer um output, criando um novo arquivo, e depois fazer um append, mostra : write error: Bad file descriptor. era so add O_WRONLY pra ele ler o arquivo e fazer o append