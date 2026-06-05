#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO "contas.dat"
#define TAM_NOME 50

typedef struct {
    int ativo;
    int numeroConta;
    char nome[TAM_NOME];
    double saldo;
} Cliente;

FILE *abrirArquivo();
long calcularPosicao(int posicao);
void limparBuffer();
void pausar();

void cadastrarCliente(FILE *arquivo);
void consultarCliente(FILE *arquivo);
void atualizarSaldo(FILE *arquivo);
void encerrarConta(FILE *arquivo);
void listarClientes(FILE *arquivo);
void restaurarLeitura(FILE *arquivo);

int main() {
    FILE *arquivo = abrirArquivo();

    if (arquivo == NULL) {
        printf("Erro ao abrir ou criar o arquivo.\n");
        return 1;
    }

    int opcao;

    do {
        printf("\n========================================\n");
        printf(" SISTEMA DE MANUTENCAO DE CONTAS\n");
        printf("========================================\n");
        printf("1. Cadastrar um novo cliente em uma posicao especifica\n");
        printf("2. Consultar um cliente pelo numero da conta\n");
        printf("3. Atualizar o saldo de um cliente\n");
        printf("4. Encerrar conta / remover cliente\n");
        printf("5. Listar todos os clientes\n");
        printf("6. Restaurar leitura do arquivo com rewind e repetir listagem\n");
        printf("7. Encerrar\n");
        printf("========================================\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                cadastrarCliente(arquivo);
                break;

            case 2:
                consultarCliente(arquivo);
                break;

            case 3:
                atualizarSaldo(arquivo);
                break;

            case 4:
                encerrarConta(arquivo);
                break;

            case 5:
                listarClientes(arquivo);
                break;

            case 6:
                restaurarLeitura(arquivo);
                break;

            case 7:
                printf("\nEncerrando o sistema...\n");
                break;

            default:
                printf("\nOpcao invalida. Tente novamente.\n");
        }

        if (opcao != 7) {
            pausar();
        }

    } while (opcao != 7);

    fclose(arquivo);

    return 0;
}

FILE *abrirArquivo() {
    FILE *arquivo = fopen(ARQUIVO, "rb+");

    if (arquivo == NULL) {
        arquivo = fopen(ARQUIVO, "wb+");
    }

    return arquivo;
}

long calcularPosicao(int posicao) {
    return (posicao - 1) * sizeof(Cliente);
}

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

void cadastrarCliente(FILE *arquivo) {
    Cliente cliente;
    Cliente existente;
    int posicao;

    printf("\n--- CADASTRAR CLIENTE ---\n");

    printf("Informe a posicao onde deseja cadastrar o cliente: ");
    scanf("%d", &posicao);
    limparBuffer();

    if (posicao <= 0) {
        printf("Posicao invalida. Use uma posicao maior que zero.\n");
        return;
    }

    fseek(arquivo, calcularPosicao(posicao), SEEK_SET);

    size_t lidos = fread(&existente, sizeof(Cliente), 1, arquivo);

    if (lidos == 1 && existente.ativo == 1) {
        printf("Ja existe um cliente ativo nessa posicao.\n");
        printf("Conta: %d\n", existente.numeroConta);
        printf("Nome: %s\n", existente.nome);
        printf("Saldo: %.2lf\n", existente.saldo);
        return;
    }

    printf("Informe o numero da conta: ");
    scanf("%d", &cliente.numeroConta);
    limparBuffer();

    printf("Informe o nome do cliente: ");
    fgets(cliente.nome, TAM_NOME, stdin);
    cliente.nome[strcspn(cliente.nome, "\n")] = '\0';

    printf("Informe o saldo inicial: ");
    scanf("%lf", &cliente.saldo);
    limparBuffer();

    cliente.ativo = 1;

    fseek(arquivo, calcularPosicao(posicao), SEEK_SET);

    fwrite(&cliente, sizeof(Cliente), 1, arquivo);
    fflush(arquivo);

    printf("\nCliente cadastrado com sucesso na posicao %d.\n", posicao);
}

void consultarCliente(FILE *arquivo) {
    Cliente cliente;
    int numeroConta;
    int encontrado = 0;

    printf("\n--- CONSULTAR CLIENTE ---\n");

    printf("Informe o numero da conta: ");
    scanf("%d", &numeroConta);
    limparBuffer();

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
            printf("\nCliente encontrado:\n");
            printf("Numero da conta: %d\n", cliente.numeroConta);
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo: %.2lf\n", cliente.saldo);

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nCliente nao encontrado.\n");
    }
}

void atualizarSaldo(FILE *arquivo) {
    Cliente cliente;
    int numeroConta;
    double novoSaldo;
    int encontrado = 0;

    printf("\n--- ATUALIZAR SALDO ---\n");

    printf("Informe o numero da conta: ");
    scanf("%d", &numeroConta);
    limparBuffer();

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
            printf("\nCliente encontrado:\n");
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo atual: %.2lf\n", cliente.saldo);

            printf("Informe o novo saldo: ");
            scanf("%lf", &novoSaldo);
            limparBuffer();

            cliente.saldo = novoSaldo;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);

            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);

            printf("\nSaldo atualizado com sucesso.\n");

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nCliente nao encontrado.\n");
    }
}

void encerrarConta(FILE *arquivo) {
    Cliente cliente;
    int numeroConta;
    int encontrado = 0;

    printf("\n--- ENCERRAR CONTA ---\n");

    printf("Informe o numero da conta que deseja encerrar: ");
    scanf("%d", &numeroConta);
    limparBuffer();

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1 && cliente.numeroConta == numeroConta) {
            printf("\nCliente encontrado:\n");
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo: %.2lf\n", cliente.saldo);

            cliente.ativo = 0;

            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);

            fwrite(&cliente, sizeof(Cliente), 1, arquivo);
            fflush(arquivo);

            printf("\nConta encerrada com sucesso.\n");

            encontrado = 1;
            break;
        }
    }

    if (!encontrado) {
        printf("\nCliente nao encontrado.\n");
    }
}

void listarClientes(FILE *arquivo) {
    Cliente cliente;
    int contador = 0;
    int posicao = 1;

    printf("\n--- LISTAGEM DE CLIENTES ---\n");

    rewind(arquivo);

    while (fread(&cliente, sizeof(Cliente), 1, arquivo) == 1) {
        if (cliente.ativo == 1) {
            printf("\nPosicao no arquivo: %d\n", posicao);
            printf("Numero da conta: %d\n", cliente.numeroConta);
            printf("Nome: %s\n", cliente.nome);
            printf("Saldo: %.2lf\n", cliente.saldo);
            printf("----------------------------------------\n");

            contador++;
        }

        posicao++;
    }

    if (contador == 0) {
        printf("Nenhum cliente ativo cadastrado.\n");
    }
}

void restaurarLeitura(FILE *arquivo) {
    printf("\n--- RESTAURANDO LEITURA COM REWIND ---\n");
    printf("O ponteiro do arquivo sera reposicionado para o inicio.\n");

    rewind(arquivo);

    printf("\nListando novamente os clientes a partir do inicio do arquivo:\n");

    listarClientes(arquivo);
}