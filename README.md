# ProcessFlow - Orquestrador de Processos em C

<img src="https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white">
<img src="https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black">
<img src="https://img.shields.io/badge/mac%20os-000000?style=for-the-badge&logo=apple&logoColor=white">
<img src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white">


Orquestrador de processos desenvolvido como projeto da disciplina de Infraestrutura de Software. O programa é responsável por cadastrar e gerenciar a execução de tarefas e programas do sistema operacional por meio de chamadas de sistema nativas Unix (`fork()`, `exec()`, `wait()`/`waitpid()`, `pipe()`, `dup2()`, `chdir()`).


## Sumário
- [Sobre o Projeto](#sobre-o-projeto)
- [Funcionalidades](#funcionalidades)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [Como Executar](#como-executar)
  - [Requisitos](#requisitos)
  - [Compilação](#compilação)
  - [Execução no Modo Interativo](#1-modo-interativo)
  - [Execução no Modo Workflow](#2-modo-workflow)
- [Guia de Comandos](#guia-de-comandos)
- [Testes e Validação](#testes-e-validação)
- [Instruções para Entrega e Submissão](#instruções-para-entrega-e-submissão)


## Sobre o Projeto

O **ProcessFlow** atua como uma shell/orquestrador de processos que gerencia a execução de comandos do terminal. O programa também gerencia o redirecionamento de entrada/saída de arquivos e o acompanhamento de processos executados em segundo plano.

O programa pode ser operado em dois modos de funcionamento:
1. **Modo Interativo:** Apresenta o prompt `processflow>` para que o usuário digite comandos diretamente no terminal.
2. **Modo Workflow:** Recebe como argumento um arquivo com extensão `.pf` contendo uma lista de comandos a serem executados em sequencia, imprimindo cada instrução na tela antes da sua execução.


## Funcionalidades

- **Cadastro de tarefas (`task`)**
- **Execução das tarefas (`run`)**
- **Execução das tarefas em sequência (`run sequential`):** Execução ordenada de uma lista de tarefas, onde cada tarefa só inicia após a conclusão da anterior.
- **Execução das tarefas em forma paralela (`run parallel`):** As tarefas são executadas ao mesmo tempo e o programa espera o término do grupo.
- **Pipe (`run pipe`):** Encadeamento de tarefas em que a saída de uma se torna a entrada da próxima.
- **Redirecionamento de entradas e saídas (`input`, `output`, `append`):** Redirecionamento da entrada padrão a partir de arquivo ou envio da saída para arquivo (sobrescrevendo ou anexando).
- **Alteração do diretório de trabalho (`workdir`):** Troca do diretório base de execução para a execução dos próximos comandos.
- **Execução em plano de fundo (`start`)** 
- **Gerenciamento de jobs (`jobs` e `wait`):** Listagem de processos ativos em segundo plano e aguardo de um job.


## Estrutura do Projeto

```text
processFlow-infrasw01/
├── processFlow.c   # Arquivo principal contendo a main e o loop dos modos interativo e workflow
├── task.h / task.c # Estrutura de dados e funções da lista encadeada de tarefas (Task)
├── run.h / run.c   # Implementação da execução de processos (fork, exec, wait, pipe, dup2, workdir)
├── job.h / job.c   # Estrutura de dados e funções para gerenciamento dos jobs em background
├── makefile        # Script de automação para compilação e limpeza do projeto
├── README.md       # Documentação do projeto
├── planejamento.md # Planejamento arquitetural e lista de requisitos
├── tentativas.md   # Registro de diário de desenvolvimento e testes
```

## Como Executar

### Requisitos:
1. **Sistema Operacional:** Linux, macOS ou Windows com ambiente WSL / MSYS2 / MinGW.
2. **Compilador C:** `gcc` ou `clang`.
3. **Ferramenta de Build:** `make`.


### Compilação

Para compilar o programa, execute no terminal:

```bash
make
```

O comando gerará o executável `processFlow`.

Para limpar os arquivos compilados e executáveis gerados:

```bash
make clean
```

---

### 1. Modo Interativo

Execute o programa sem nenhum argumento adicional:

```bash
./processFlow
```

O prompt `processflow>` será exibido para digitação de comandos:

```text
processflow> task listar /bin/ls -l
processflow> run listar
processflow> exit
```

---

### 2. Modo Workflow

Execute o programa passando o caminho de um arquivo de comandos `.pf`:

```bash
./processFlow testes/teste3/teste3-entrada.txt
```

Cada linha do arquivo será impressa na tela antes de ser processada e o prompt (`processflow> `) não será exibido.

---

## Guia de Comandos

| Comando | Sintaxe | Descrição |
| :--- | :--- | :--- |
| **`task`** | `task <nome> <programa> [argumentos...]` | Cadastra uma nova tarefa na memória. |
| **`run`** | `run <nome_tarefa>` | Executa uma tarefa cadastrada. |
| **`run sequential`** | `run sequential <t1> <t2> ...` | Executa uma sequência de tarefas ordenadamente. |
| **`run parallel`** | `run parallel <t1> <t2> ...` | Executa múltiplas tarefas em paralelo. |
| **`run pipe`** | `run pipe <t1> <t2> <t3>` | Conecta tarefas via pipeline (`\|`). |
| **`input`** | `input <tarefa> <arquivo.txt>` | Redireciona a entrada padrão (`<`) da tarefa. |
| **`output`** | `output <tarefa> <arquivo.txt>` | Redireciona a saída padrão (`>`) da tarefa (sobrescreve). |
| **`append`** | `append <tarefa> <arquivo.txt>` | Redireciona a saída padrão (`>>`) da tarefa (anexa). |
| **`workdir`** | `workdir <caminho_diretorio>` | Altera o diretório de trabalho atual. |
| **`start`** | `start <tarefa>` | Inicia a tarefa em segundo plano (*background*). |
| **`jobs`** | `jobs` | Lista os jobs atualmente rodando em background. |
| **`wait`** | `wait <jobId>` | Aguarda a finalização de um job específico pelo ID. |
| **`exit`** | `exit` | Encerra a sessão do ProcessFlow. |
