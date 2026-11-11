#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>

#define ARQUIVO_CLIENTES "clientes.txt"

typedef struct
{
    int id;
    char nome[50];
    double salario;
    char dataNascimento[11];
    char profissao[50];
    char telefone[20];
    char empresa[80];
} Cliente;

typedef struct
{
    Cliente *clientes;
    size_t quantidade;
    size_t capacidade;
    int proximoId;
} Banco;

static void removerQuebraLinha(char *texto)
{
    size_t tamanho = strlen(texto);
    if (tamanho > 0 && texto[tamanho - 1] == '\n')
        texto[tamanho - 1] = '\0';
}

static void removerEspacosDasPontas(char *texto)
{
    char *primeiroCaractereUtil = texto;
    while (isspace((unsigned char)*primeiroCaractereUtil))
        primeiroCaractereUtil++;
    memmove(texto, primeiroCaractereUtil, strlen(primeiroCaractereUtil) + 1);

    size_t tamanho = strlen(texto);
    while (tamanho > 0 && isspace((unsigned char)texto[tamanho - 1]))
        texto[--tamanho] = '\0';
}

static int lerLinha(const char *mensagem, char *destino, size_t tamanhoDestino)
{
    printf("%s", mensagem);
    if (!fgets(destino, (int)tamanhoDestino, stdin))
        return 0;

    if (strchr(destino, '\n') == NULL)
    {
        int caractere;
        while ((caractere = getchar()) != '\n' && caractere != EOF)
            ;
    }
    removerQuebraLinha(destino);
    return 1;
}

static int lerTextoObrigatorio(const char *mensagem, char *destino, size_t tamanhoDestino)
{
    char entrada[256];
    for (;;)
    {
        if (!lerLinha(mensagem, entrada, sizeof entrada))
            return 0;
        removerEspacosDasPontas(entrada);

        if (entrada[0] == '\0')
            printf("  Campo obrigatorio.\n");
        else if (strchr(entrada, ';'))
            printf("  O caractere ';' nao e permitido.\n");
        else if (strlen(entrada) >= tamanhoDestino)
            printf("  Texto muito longo (maximo %zu caracteres).\n", tamanhoDestino - 1);
        else
        {
            strcpy(destino, entrada);
            return 1;
        }
    }
}

static int lerInteiro(const char *mensagem, int *numeroLido)
{
    char entrada[32];
    char *fimDaConversao;

    if (!lerLinha(mensagem, entrada, sizeof entrada))
        return 0;

    long valor = strtol(entrada, &fimDaConversao, 10);
    if (fimDaConversao == entrada || *fimDaConversao != '\0')
        *numeroLido = -1;
    else
        *numeroLido = (int)valor;
    return 1;
}

static int lerSalario(double *salarioLido)
{
    char entrada[64];
    char *fimDaConversao;

    for (;;)
    {
        if (!lerLinha("Salario: ", entrada, sizeof entrada))
            return 0;

        for (char *caractere = entrada; *caractere; caractere++)
            if (*caractere == ',')
                *caractere = '.';

        double valor = strtod(entrada, &fimDaConversao);
        if (fimDaConversao != entrada && *fimDaConversao == '\0' && valor >= 0)
        {
            *salarioLido = valor;
            return 1;
        }
        printf("  Salario invalido. Use um numero positivo (ex: 2500.50).\n");
    }
}

static int anoBissexto(int ano)
{
    return (ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0;
}

static int dataValida(const char *data)
{
    int dia, mes, ano;
    char caractereExtra;

    if (strlen(data) != 10 || data[2] != '/' || data[5] != '/')
        return 0;
    if (sscanf(data, "%2d/%2d/%4d%c", &dia, &mes, &ano, &caractereExtra) != 3)
        return 0;
    if (ano < 1900 || ano > 2100 || mes < 1 || mes > 12 || dia < 1)
        return 0;

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && anoBissexto(ano))
        diasPorMes[1] = 29;

    return dia <= diasPorMes[mes - 1];
}

static int lerDataNascimento(char *destino)
{
    char entrada[64];
    for (;;)
    {
        if (!lerLinha("Data de nascimento (dd/mm/aaaa): ", entrada, sizeof entrada))
            return 0;
        removerEspacosDasPontas(entrada);

        if (dataValida(entrada))
        {
            strcpy(destino, entrada);
            return 1;
        }
        printf("  Data invalida.\n");
    }
}

static int lerTelefone(char *destino)
{
    char entrada[64];
    for (;;)
    {
        if (!lerLinha("Telefone (com DDD): ", entrada, sizeof entrada))
            return 0;

        char digitos[20];
        size_t quantidadeDigitos = 0;
        for (char *caractere = entrada; *caractere; caractere++)
            if (isdigit((unsigned char)*caractere) && quantidadeDigitos < sizeof digitos - 1)
                digitos[quantidadeDigitos++] = *caractere;
        digitos[quantidadeDigitos] = '\0';

        if (quantidadeDigitos == 10 || quantidadeDigitos == 11)
        {
            strcpy(destino, digitos);
            return 1;
        }
        printf("  Telefone invalido. Informe DDD + numero (10 ou 11 digitos).\n");
    }
}

static int salvarClientesNoArquivo(const Banco *banco)
{
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "w");
    if (!arquivo)
        return 0;

    for (size_t indice = 0; indice < banco->quantidade; indice++)
    {
        const Cliente *cliente = &banco->clientes[indice];
        fprintf(arquivo, "%d;%s;%.2f;%s;%s;%s;%s\n",
                cliente->id, cliente->nome, cliente->salario,
                cliente->dataNascimento, cliente->profissao,
                cliente->telefone, cliente->empresa);
    }
    fclose(arquivo);
    return 1;
}

static int garantirEspacoParaNovoCliente(Banco *banco)
{
    if (banco->quantidade < banco->capacidade)
        return 1;

    size_t novaCapacidade = banco->capacidade ? banco->capacidade * 2 : 8;
    Cliente *novoVetor = realloc(banco->clientes, novaCapacidade * sizeof *novoVetor);
    if (!novoVetor)
        return 0;

    banco->clientes = novoVetor;
    banco->capacidade = novaCapacidade;
    return 1;
}

static void carregarClientesDoArquivo(Banco *banco)
{
    FILE *arquivo = fopen(ARQUIVO_CLIENTES, "r");
    char linhaDoArquivo[512];

    if (!arquivo)
        return;

    while (fgets(linhaDoArquivo, sizeof linhaDoArquivo, arquivo))
    {
        Cliente cliente;
        int camposLidos = sscanf(linhaDoArquivo,
                                 "%d;%49[^;];%lf;%10[^;];%49[^;];%19[^;];%79[^\n]",
                                 &cliente.id, cliente.nome, &cliente.salario,
                                 cliente.dataNascimento, cliente.profissao,
                                 cliente.telefone, cliente.empresa);
        if (camposLidos != 7)
            continue;

        if (!garantirEspacoParaNovoCliente(banco))
            break;

        banco->clientes[banco->quantidade++] = cliente;
        if (cliente.id >= banco->proximoId)
            banco->proximoId = cliente.id + 1;
    }
    fclose(arquivo);
}

static void exibirCliente(const Cliente *cliente)
{
    printf("  ID: %d\n", cliente->id);
    printf("  Nome: %s\n", cliente->nome);
    printf("  Salario: R$ %.2f\n", cliente->salario);
    printf("  Nascimento: %s\n", cliente->dataNascimento);
    printf("  Profissao: %s\n", cliente->profissao);

    if (strlen(cliente->telefone) == 11)
        printf("  Telefone: (%.2s) %.5s-%s\n",
               cliente->telefone, cliente->telefone + 2, cliente->telefone + 7);
    else
        printf("  Telefone: (%.2s) %.4s-%s\n",
               cliente->telefone, cliente->telefone + 2, cliente->telefone + 6);

    printf("  Empresa: %s\n", cliente->empresa);
}

static void cadastrarCliente(Banco *banco)
{
    Cliente novoCliente;
    memset(&novoCliente, 0, sizeof novoCliente);

    printf("\n--- Dados do cliente ---\n");
    if (!lerTextoObrigatorio("Nome: ", novoCliente.nome, sizeof novoCliente.nome) ||
        !lerSalario(&novoCliente.salario) ||
        !lerDataNascimento(novoCliente.dataNascimento) ||
        !lerTextoObrigatorio("Profissao: ", novoCliente.profissao, sizeof novoCliente.profissao) ||
        !lerTelefone(novoCliente.telefone) ||
        !lerTextoObrigatorio("Empresa: ", novoCliente.empresa, sizeof novoCliente.empresa))
    {
        printf("\nEntrada encerrada. Cadastro cancelado.\n");
        return;
    }

    if (!garantirEspacoParaNovoCliente(banco))
    {
        printf("\nErro: memoria insuficiente.\n");
        return;
    }

    novoCliente.id = banco->proximoId++;
    banco->clientes[banco->quantidade++] = novoCliente;

    if (salvarClientesNoArquivo(banco))
        printf("\nConta cadastrada com sucesso!\n");
    else
        printf("\nConta cadastrada, mas nao foi possivel gravar no arquivo.\n");
    exibirCliente(&novoCliente);
}

static void listarClientes(const Banco *banco)
{
    if (banco->quantidade == 0)
    {
        printf("\nNenhum cliente cadastrado.\n");
        return;
    }

    printf("\n--- %zu cliente(s) ---\n", banco->quantidade);
    for (size_t indice = 0; indice < banco->quantidade; indice++)
    {
        exibirCliente(&banco->clientes[indice]);
        printf("\n");
    }
}

static int textoContemTrecho(const char *texto, const char *trecho)
{
    size_t tamanhoTrecho = strlen(trecho);

    for (; *texto; texto++)
    {
        size_t caracteresIguais = 0;
        while (caracteresIguais < tamanhoTrecho && texto[caracteresIguais] &&
               tolower((unsigned char)texto[caracteresIguais]) ==
                   tolower((unsigned char)trecho[caracteresIguais]))
            caracteresIguais++;

        if (caracteresIguais == tamanhoTrecho)
            return 1;
    }
    return 0;
}

static void buscarClientePorNome(const Banco *banco)
{
    char nomeBuscado[50];
    int encontrouAlgum = 0;

    if (!lerLinha("\nBuscar por nome: ", nomeBuscado, sizeof nomeBuscado))
        return;
    removerEspacosDasPontas(nomeBuscado);
    if (nomeBuscado[0] == '\0')
        return;

    for (size_t indice = 0; indice < banco->quantidade; indice++)
    {
        if (textoContemTrecho(banco->clientes[indice].nome, nomeBuscado))
        {
            printf("\n");
            exibirCliente(&banco->clientes[indice]);
            encontrouAlgum = 1;
        }
    }

    if (!encontrouAlgum)
        printf("\nNenhum cliente encontrado.\n");
}

static void removerCliente(Banco *banco)
{
    int idBuscado;
    char confirmacao[8];

    if (!lerInteiro("\nID do cliente a remover: ", &idBuscado))
        return;

    for (size_t indice = 0; indice < banco->quantidade; indice++)
    {
        if (banco->clientes[indice].id != idBuscado)
            continue;

        exibirCliente(&banco->clientes[indice]);
        if (!lerLinha("Confirmar remocao? (s/N): ", confirmacao, sizeof confirmacao))
            return;

        if (tolower((unsigned char)confirmacao[0]) != 's')
        {
            printf("Operacao cancelada.\n");
            return;
        }

        size_t clientesAposORemovido = banco->quantidade - indice - 1;
        memmove(&banco->clientes[indice], &banco->clientes[indice + 1],
                clientesAposORemovido * sizeof(Cliente));
        banco->quantidade--;
        salvarClientesNoArquivo(banco);
        printf("Cliente removido.\n");
        return;
    }

    printf("Cliente nao encontrado.\n");
}

int main(void)
{
    Banco banco = {NULL, 0, 0, 1};
    int opcaoMenu;

    setlocale(LC_ALL, "");
    carregarClientesDoArquivo(&banco);

    do
    {
        printf("\n===== BANCO DO POVO =====\n");
        printf("1 - Cadastrar conta\n");
        printf("2 - Listar clientes\n");
        printf("3 - Buscar por nome\n");
        printf("4 - Remover cliente\n");
        printf("0 - Sair\n");

        if (!lerInteiro("Digite a opcao desejada: ", &opcaoMenu))
            break;

        switch (opcaoMenu)
        {
        case 1: cadastrarCliente(&banco);       break;
        case 2: listarClientes(&banco);         break;
        case 3: buscarClientePorNome(&banco);   break;
        case 4: removerCliente(&banco);         break;
        case 0: printf("\nAte logo!\n");        break;
        default: printf("\nOpcao invalida!\n");
        }
    } while (opcaoMenu != 0);

    free(banco.clientes);
    return 0;
}