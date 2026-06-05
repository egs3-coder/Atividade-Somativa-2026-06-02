# Atividade-Somativa-2026-06-02

# Sistema de Manutenção de Contas em Arquivo Binário

## 1. Descrição do projeto

Este projeto foi desenvolvido em linguagem C com o objetivo de criar um sistema simples de manutenção de contas bancárias utilizando arquivo binário.

O sistema permite cadastrar, consultar, atualizar, remover e listar clientes armazenados em um arquivo. Cada cliente é salvo como um registro de tamanho fixo dentro do arquivo binário.

O exercício exige o uso dos seguintes recursos:

* Arquivo binário;
* Registros de tamanho fixo;
* Acesso direto a posições específicas com `fseek()`;
* Leitura de registros com `fread()`;
* Escrita de registros com `fwrite()`;
* Retorno da leitura ao início do arquivo com `rewind()`.

O programa possui um menu com as seguintes opções:

1. Cadastrar um novo cliente em uma posição específica.
2. Consultar um cliente pelo número da conta.
3. Atualizar o saldo de um cliente.
4. Encerrar conta, removendo logicamente o cliente.
5. Listar todos os clientes.
6. Restaurar a leitura do arquivo do início com `rewind()` para repetir a listagem.
7. Encerrar o programa.

---

## 2. Estrutura do cliente

Cada cliente é representado por uma estrutura chamada `Cliente`.

```c
typedef struct {
    int ativo;
    int numeroConta;
    char nome[50];
    double saldo;
} Cliente;
```

A estrutura possui os seguintes campos:

### Campo `ativo`

```c
int ativo;
```

Esse campo indica se o registro está ativo ou removido.

* `ativo = 1`: cliente ativo;
* `ativo = 0`: cliente removido ou conta encerrada.

Essa estratégia foi usada porque, em arquivos binários com registros de tamanho fixo, remover fisicamente um registro do meio do arquivo não é simples. Por isso, o programa apenas marca o cliente como inativo.

---

### Campo `numeroConta`

```c
int numeroConta;
```

Armazena o número da conta do cliente. Esse número é usado nas opções de consulta, atualização de saldo e encerramento de conta.

---

### Campo `nome`

```c
char nome[50];
```

Armazena o nome do cliente. Foi definido um tamanho fixo de 50 caracteres para manter todos os registros com o mesmo tamanho dentro do arquivo binário.

---

### Campo `saldo`

```c
double saldo;
```

Armazena o saldo da conta do cliente.

---

## 3. Por que o arquivo é binário?

O arquivo utilizado no programa é o arquivo:

```c
contas.dat
```

Ele é aberto em modo binário. O uso de arquivo binário permite salvar a estrutura inteira diretamente no arquivo, mantendo os dados no mesmo formato em que estão na memória.

A função usada para abrir o arquivo é:

```c
FILE *abrirArquivo() {
    FILE *arquivo = fopen(ARQUIVO, "rb+");

    if (arquivo == NULL) {
        arquivo = fopen(ARQUIVO, "wb+");
    }

    return arquivo;
}
```

Primeiro, o programa tenta abrir o arquivo em modo:

```c
rb+
```

Esse modo significa:

* `r`: leitura;
* `b`: binário;
* `+`: leitura e escrita.

Caso o arquivo ainda não exista, ele é criado com:

```c
wb+
```

Esse modo cria um novo arquivo binário para leitura e escrita.

---

## 4. Registros de tamanho fixo

O exercício pede registros de tamanho fixo. Isso foi feito usando uma `struct` com campos de tamanho definido.

Cada registro ocupa exatamente:

```c
sizeof(Cliente)
```

Assim, é possível calcular a posição exata de qualquer cliente dentro do arquivo.

Por exemplo:

* Posição 1 começa no byte 0;
* Posição 2 começa depois de 1 registro;
* Posição 3 começa depois de 2 registros;
* E assim por diante.

A função usada para calcular a posição é:

```c
long calcularPosicao(int posicao) {
    return (posicao - 1) * sizeof(Cliente);
}
```

Se o usuário quiser cadastrar um cliente na posição 3, o cálculo será:

```c
(3 - 1) * sizeof(Cliente)
```

Ou seja, o ponteiro do arquivo será movido para o local exato onde o terceiro registro deve ficar.

---

## 5. Uso da função `fseek()`

A função `fseek()` foi usada para acessar posições específicas dentro do arquivo.

No cadastro de cliente, por exemplo:

```c
fseek(arquivo, calcularPosicao(posicao), SEEK_SET);
```

Essa linha move o ponteiro do arquivo para a posição escolhida pelo usuário.

O parâmetro `SEEK_SET` significa que a movimentação começa a partir do início do arquivo.

Depois disso, o programa pode ler ou escrever diretamente naquela posição.

---

## 6. Uso da função `fread()`

A função `fread()` foi usada para ler registros do arquivo binário.

Exemplo:

```c
fread(&cliente, sizeof(Cliente), 1, arquivo);
```

Essa instrução lê um registro do tipo `Cliente` do arquivo.

Os parâmetros são:

1. `&cliente`: endereço da variável onde o registro será armazenado;
2. `sizeof(Cliente)`: tamanho do registro;
3. `1`: quantidade de registros a serem lidos;
4. `arquivo`: ponteiro para o arquivo.

No programa, `fread()` é usada principalmente para:

* verificar se uma posição já está ocupada;
* consultar cliente pelo número da conta;
* listar clientes;
* localizar cliente para atualizar saldo;
* localizar cliente para encerrar conta.

---

## 7. Uso da função `fwrite()`

A função `fwrite()` foi usada para gravar registros no arquivo binário.

Exemplo:

```c
fwrite(&cliente, sizeof(Cliente), 1, arquivo);
```

Essa instrução grava um registro completo do tipo `Cliente` no arquivo.

Os parâmetros são:

1. `&cliente`: endereço da variável que será gravada;
2. `sizeof(Cliente)`: tamanho do registro;
3. `1`: quantidade de registros a serem gravados;
4. `arquivo`: ponteiro para o arquivo.

No programa, `fwrite()` é usada para:

* cadastrar cliente;
* atualizar saldo;
* marcar uma conta como encerrada.

Após cada gravação, foi usado:

```c
fflush(arquivo);
```

Isso força a gravação dos dados no arquivo imediatamente.

---

## 8. Uso da função `rewind()`

A função `rewind()` foi usada para retornar o ponteiro do arquivo ao início.

Exemplo:

```c
rewind(arquivo);
```

Isso é importante porque, após uma leitura ou escrita, o ponteiro do arquivo pode estar no final ou em outra posição. Para listar todos os clientes desde o início, é necessário reposicionar o ponteiro.

A opção 6 do menu demonstra exatamente esse requisito do exercício:

```c
void restaurarLeitura(FILE *arquivo) {
    printf("\n--- RESTAURANDO LEITURA COM REWIND ---\n");
    printf("O ponteiro do arquivo sera reposicionado para o inicio.\n");

    rewind(arquivo);

    printf("\nListando novamente os clientes a partir do inicio do arquivo:\n");

    listarClientes(arquivo);
}
```

Essa opção reposiciona o ponteiro no início do arquivo e chama a função de listagem novamente.

---

## 9. Explicação das opções do menu

O menu principal do programa apresenta as opções solicitadas no enunciado.

```c
printf("1. Cadastrar um novo cliente em uma posicao especifica\n");
printf("2. Consultar um cliente pelo numero da conta\n");
printf("3. Atualizar o saldo de um cliente\n");
printf("4. Encerrar conta / remover cliente\n");
printf("5. Listar todos os clientes\n");
printf("6. Restaurar leitura do arquivo com rewind e repetir listagem\n");
printf("7. Encerrar\n");
```

A estrutura de repetição `do while` mantém o menu funcionando até o usuário escolher a opção 7.

---

## 10. Opção 1: Cadastrar cliente em posição específica

A opção de cadastro pede ao usuário uma posição no arquivo.

Exemplo:

```c
printf("Informe a posicao onde deseja cadastrar o cliente: ");
scanf("%d", &posicao);
```

Depois, o programa usa `fseek()` para ir diretamente até aquela posição:

```c
fseek(arquivo, calcularPosicao(posicao), SEEK_SET);
```

Em seguida, o programa tenta ler o registro já existente naquela posição:

```c
size_t lidos = fread(&existente, sizeof(Cliente), 1, arquivo);
```

Se já existir um cliente ativo na posição, o sistema não permite sobrescrever.

Caso a posição esteja vazia ou possua um cliente inativo, o novo cliente é cadastrado.

O programa solicita:

* número da conta;
* nome do cliente;
* saldo inicial.

Depois grava o cliente no arquivo com:

```c
fwrite(&cliente, sizeof(Cliente), 1, arquivo);
```

---

## 11. Opção 2: Consultar cliente pelo número da conta

A consulta percorre o arquivo desde o início usando `rewind()`.

```c
rewind(arquivo);
```

Depois, o programa lê registro por registro com `fread()`.

```c
while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
```

Para cada cliente lido, o programa verifica se:

```c
cliente.ativo == 1 && cliente.numeroConta == numeroConta
```

Ou seja, ele verifica se o cliente está ativo e se o número da conta é igual ao número pesquisado.

Se encontrar, exibe:

* número da conta;
* nome;
* saldo.

---

## 12. Opção 3: Atualizar saldo

A atualização de saldo também começa lendo o arquivo desde o início:

```c
rewind(arquivo);
```

O programa procura o cliente pelo número da conta.

Quando encontra o cliente, pede o novo saldo:

```c
scanf("%lf", &novoSaldo);
```

Depois atualiza o campo:

```c
cliente.saldo = novoSaldo;
```

Como o `fread()` já avançou o ponteiro para depois do registro lido, é necessário voltar uma posição antes de gravar novamente.

Isso é feito com:

```c
fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
```

O parâmetro `SEEK_CUR` significa que o deslocamento será feito a partir da posição atual do ponteiro.

Depois o registro atualizado é gravado com:

```c
fwrite(&cliente, sizeof(Cliente), 1, arquivo);
```

---

## 13. Opção 4: Encerrar conta

A opção de encerrar conta remove logicamente o cliente.

O programa localiza o cliente pelo número da conta. Quando encontra, altera:

```c
cliente.ativo = 0;
```

Isso significa que o cliente não será mais considerado nas consultas e listagens.

Assim como na atualização de saldo, o ponteiro precisa voltar uma posição antes de sobrescrever o registro:

```c
fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
```

Depois o registro é gravado novamente com:

```c
fwrite(&cliente, sizeof(Cliente), 1, arquivo);
```

A conta fica encerrada, mas o registro continua ocupando o mesmo espaço no arquivo. Isso mantém o arquivo com registros de tamanho fixo.

---

## 14. Opção 5: Listar todos os clientes

A listagem usa `rewind()` para começar a leitura do início do arquivo.

```c
rewind(arquivo);
```

Depois lê todos os registros com:

```c
while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1)
```

Somente clientes ativos são exibidos:

```c
if (cliente.ativo == 1)
```

Para cada cliente ativo, o programa exibe:

* posição no arquivo;
* número da conta;
* nome;
* saldo.

Clientes encerrados não aparecem na listagem.

---

## 15. Opção 6: Restaurar leitura com `rewind()`

Essa opção foi criada especificamente para atender ao requisito do exercício.

Ela mostra que o programa consegue reposicionar o ponteiro do arquivo no início usando:

```c
rewind(arquivo);
```

Depois disso, a listagem é repetida chamando:

```c
listarClientes(arquivo);
```

Assim, o usuário consegue visualizar novamente todos os clientes ativos desde o início do arquivo.

---

## 16. Opção 7: Encerrar

A opção 7 encerra o laço do menu e finaliza o programa.

Antes de terminar, o arquivo é fechado com:

```c
fclose(arquivo);
```

Fechar o arquivo é importante para liberar recursos do sistema e garantir que todos os dados sejam salvos corretamente.

---

## 17. Como compilar o programa

Para compilar usando GCC, execute o comando:

```bash
gcc main.c -o sistema_contas
```

---

## 18. Como executar o programa

Depois de compilar, execute:

No Linux ou macOS:

```bash
./sistema_contas
```

No Windows:

```bash
sistema_contas.exe
```

---

## 19. Exemplo de uso

Ao iniciar o programa, o menu será exibido:

```text
========================================
 SISTEMA DE MANUTENCAO DE CONTAS
========================================
1. Cadastrar um novo cliente em uma posicao especifica
2. Consultar um cliente pelo numero da conta
3. Atualizar o saldo de um cliente
4. Encerrar conta / remover cliente
5. Listar todos os clientes
6. Restaurar leitura do arquivo com rewind e repetir listagem
7. Encerrar
========================================
Escolha uma opcao:
```

Exemplo de cadastro:

```text
Informe a posicao onde deseja cadastrar o cliente: 1
Informe o numero da conta: 1001
Informe o nome do cliente: Maria Silva
Informe o saldo inicial: 1500.50
```

Depois, ao listar os clientes, será exibido:

```text
Posicao no arquivo: 1
Numero da conta: 1001
Nome: Maria Silva
Saldo: 1500.50
```

---

## 20. Organização dos arquivos no repositório

O repositório pode ficar organizado assim:

```text
sistema-manutencao-contas/
│
├── main.c
├── README.md
└── contas.dat
```

Observação: o arquivo `contas.dat` será criado automaticamente quando o programa for executado. Portanto, não é obrigatório enviá-lo para o GitHub.

---
