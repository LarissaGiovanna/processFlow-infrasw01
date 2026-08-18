# Arquitetura esperada:

```
lgsc/
|
|-- .C
|-- .h
|--README
|--makefile
```
no final ser compactado para .tar -> lgsc.tar
```bash
tar -cvf nome_do_arquivo.tar pasta_a_ser_compactada/   
```

RELATORIO -> .pdf

pdf, tar e link github -> enviados no classroom

# Como sera
- O orquestrador vai automatizar e coordenar tarefas em multiplos sistemas de forma centralizada, atuando como um maestro (IA search)
- Arquivo em C que recebe as tasks via args

## modos diferentes
### interativo
- deve executar e aparecer no terminal (processflow>) ???
- usuario deve digitar os comandos 
```bash
Sintaxe:
processflow> task <nome> <programa> [argumentos...]

Ex:
processflow> task listar /bin/ls -l
```
- é necessario para criar uma nova tarefa

### workflow
- recebe um arquivo .pf como argumento, contendo comandos para ser executados
- é o modo interativo mas especifico para receber arquivos e trata-los
- o prompt (processflow>) nao sera exibido
- cada linha do arquivo lido devera ser printada na tela antes de ser executada

## comandos
- para ambos os modos, para sair, sera o comando `exit`
- para executar as tarefas cadastradas, devera ser executada com `run <nome_tarefa>`
### modos execucao
- mais de uma tarefa pode ser passada no comando e ser executada:
#### sequencial
- a tarefa deve **somente iniciar quando a anterior terminar** (avisar quando terminar para poder passar para a proxima)
```bash
Sintaze:
processflow> run sequential tarefa1 tarefa2 tarefa3
```
#### paralelo
- todas as tarefas devem ser iniciadas antes que o ProcessFlow espere pelo término do grupo. ???
```bash
Sintaxe:
processflow> run parallel tarefa1 tarefa2 tarefa3
```

## deve suportar
### Pipe ( | )
- a saida de uma tarefa pode ser enviada como a entrada de outra, como em um | (todas as tarefas devem ter um return ???)
```bash
Sintaxe:
processflow> run pipe <tarefa1> <tarefa2> <tarefa3>
Ex:
processflow> run pipe listar ordenar contar
```

### redirecionamento ( > )
- uma tarefa pode receber sua entrada DE um arquivo OU enviar SUA saida para um arquivo
```bash
EX:
processflow> input ordenar nomes.txt //ordenar recebe como entrada oq esta em nomes.txt ???
processflow> output ordenar resultado.txt //a saida de ordenar vai para resultado.txt ???
processflow> append ordenar historico.txt //adiciona a saida de ordenar em historico.txt
```

### diretorios de trabalho
- comando `workdir` serve como uma junção do `mkdir` e do `cd` e altera o diretório utilizado pelas tarefas executadas posteriormente. Ex: `workdir pasta/` e `run tarefa1`, o comando run sera executado dentro da pasta. ???

![EXEMPLO CODIGO FORK E WAIT EM C PAG118](image.png)
