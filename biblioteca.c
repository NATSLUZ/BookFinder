#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <stdlib.h>
#include <ctype.h>

#ifdef _WIN32
    #define LIMPAR "cls"
#else
    #define LIMPAR "clear"
#endif

#define MAX_LIVROS 100

typedef struct {
    int codigo;
    char titulo[100];
    char autor[100];
    char genero[50];
    int ano;
} Livro;

Livro biblioteca[MAX_LIVROS];
int totalLivros = 0;

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para limpar a tela
void limparTela() {
    system(LIMPAR);
}

// Exibe o menu principal
void exibirMenu() {
    printf("====================================================================================\n");
    printf("        Bem vindo ao Sistema BookFinder, o seu gerenciador virtual de livros        \n");
    printf("====================================================================================\n");
    printf("1. Adicionar Livro\n");
    printf("2. Remover Livro\n");
    printf("3. Exibir todos os livros (ordem alfabética de título)\n");
    printf("4. Pesquisar livro (digite qualquer detalhe)\n");
    printf("5. Sair\n");
    printf("====================================================================================\n");
    printf("Escolha uma opção: ");
}

// Função que retorna o menor código disponível (reutilizando os que foram removidos)
int obterCodigoDisponivel() {
    int codigo = 1;
    int encontrado;
    while (1) {
        encontrado = 0;
        for (int i = 0; i < totalLivros; i++) {
            if (biblioteca[i].codigo == codigo) {
                encontrado = 1;
                break;
            }
        }
        if (!encontrado)
            return codigo;
        codigo++;
    }
}

// Função auxiliar para comparação case-insensitive
int containsIgnoreCase(const char* str, const char* sub) {
    char strLower[256], subLower[256];
    int i;
    for (i = 0; str[i] && i < 255; i++) {
        strLower[i] = tolower((unsigned char)str[i]);
    }
    strLower[i] = '\0';
    for (i = 0; sub[i] && i < 255; i++) {
        subLower[i] = tolower((unsigned char)sub[i]);
    }
    subLower[i] = '\0';
    return strstr(strLower, subLower) != NULL;
}

void adicionarLivro() {
    if (totalLivros >= MAX_LIVROS) {
        printf("\nA biblioteca está cheia!\n");
        return;
    }

    Livro novoLivro;
    novoLivro.codigo = obterCodigoDisponivel();

    printf("\nDigite o título do livro: ");
    limparBuffer();
    fgets(novoLivro.titulo, 100, stdin);
    strtok(novoLivro.titulo, "\n");

    printf("Digite o autor: ");
    fgets(novoLivro.autor, 100, stdin);
    strtok(novoLivro.autor, "\n");

    printf("Digite o gênero: ");
    fgets(novoLivro.genero, 50, stdin);
    strtok(novoLivro.genero, "\n");

    printf("Digite o ano de publicação: ");
    scanf("%d", &novoLivro.ano);

    biblioteca[totalLivros++] = novoLivro;
    printf("\nLivro adicionado com sucesso! Código: %04d\n", novoLivro.codigo);

    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

void removerLivro() {
    int cod;
    printf("\nDigite o código do livro a ser removido: ");
    scanf("%d", &cod);

    int indice = -1;
    for (int i = 0; i < totalLivros; i++) {
        if (biblioteca[i].codigo == cod) {
            indice = i;
            break;
        }
    }
    if (indice == -1) {
        printf("\nLivro não encontrado!\n");
    } else {
        printf("\nDetalhes do livro:\n");
        printf("Código: %04d\n", biblioteca[indice].codigo);
        printf("Título: %s\n", biblioteca[indice].titulo);
        printf("Autor: %s\n", biblioteca[indice].autor);
        printf("Gênero: %s\n", biblioteca[indice].genero);
        printf("Ano: %d\n", biblioteca[indice].ano);

        printf("\nDeseja remover este livro? (S/N): ");
        char confirm;
        limparBuffer();
        confirm = getchar();
        if (confirm == 'S' || confirm == 's') {
            // Remove deslocando os livros subsequentes
            for (int i = indice; i < totalLivros - 1; i++) {
                biblioteca[i] = biblioteca[i + 1];
            }
            totalLivros--;
            printf("\nLivro removido com sucesso!\n");
        } else {
            printf("\nOperação de remoção cancelada.\n");
        }
    }
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

void listarTodosOrdenado() {
    if (totalLivros == 0) {
        printf("\nNenhum livro cadastrado.\n");
    } else {
        // Cria uma cópia da biblioteca para não alterar a ordem original
        Livro temp[MAX_LIVROS];
        for (int i = 0; i < totalLivros; i++) {
            temp[i] = biblioteca[i];
        }

        // Ordena pelo título (ordem alfabética)
        for (int i = 0; i < totalLivros - 1; i++) {
            for (int j = i + 1; j < totalLivros; j++) {
                if (strcmp(temp[i].titulo, temp[j].titulo) > 0) {
                    Livro aux = temp[i];
                    temp[i] = temp[j];
                    temp[j] = aux;
                }
            }
        }

        printf("\nLivros em ordem alfabética de título:\n");
        printf("--------------------------------------------------------------------------------\n");
        for (int i = 0; i < totalLivros; i++) {
            printf("Código: %04d | Título: %s | Autor: %s | Gênero: %s | Ano: %d\n",
                   temp[i].codigo, temp[i].titulo, temp[i].autor, temp[i].genero, temp[i].ano);
        }
        printf("--------------------------------------------------------------------------------\n");
    }
    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

void pesquisarLivro() {
    char busca[100];
    char continuar;
    do {
        // Limpa a tela a cada nova pesquisa para evitar poluição visual
        limparTela();
        printf("\nDigite qualquer detalhe do livro (ex.: código, ano, autor ou título): ");
        limparBuffer();
        fgets(busca, 100, stdin);
        strtok(busca, "\n");

        int num = atoi(busca); // Converte para número, se possível
        int encontrado = 0;

        printf("\nResultado da pesquisa:\n");
        printf("-------------------------------------------------------------\n");
        for (int i = 0; i < totalLivros; i++) {
            if (containsIgnoreCase(biblioteca[i].titulo, busca) ||
                containsIgnoreCase(biblioteca[i].autor, busca) ||
                containsIgnoreCase(biblioteca[i].genero, busca) ||
                biblioteca[i].codigo == num ||
                biblioteca[i].ano == num) {

                printf("Código: %04d | Título: %s | Autor: %s | Gênero: %s | Ano: %d\n",
                       biblioteca[i].codigo, biblioteca[i].titulo, biblioteca[i].autor,
                       biblioteca[i].genero, biblioteca[i].ano);
                encontrado = 1;
            }
        }
        if (!encontrado) {
            printf("Nenhum livro encontrado com os detalhes informados.\n");
        }
        printf("------------------------------------------------------------\n");

        // Lê a resposta para realizar outra pesquisa
        char resp[10];
        printf("\nDeseja realizar outra pesquisa? (S/N): ");
        fgets(resp, 10, stdin);
        continuar = tolower(resp[0]);
    } while (continuar == 's');

    printf("\nPressione ENTER para continuar...");
    limparBuffer();
    getchar();
}

int main() {
    setlocale(LC_ALL, ""); // Suporte a caracteres acentuados
    int opcao;

    do {
        limparTela();
        exibirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                limparTela();
                adicionarLivro();
                break;
            case 2:
                limparTela();
                removerLivro();
                break;
            case 3:
                limparTela();
                listarTodosOrdenado();
                break;
            case 4:
                pesquisarLivro();
                break;
            case 5:
                limparTela();
                printf("\nObrigado por usar o programa!\n");
                printf("Natan Luz, UNEB Bahia, 2025\n");
                printf("\nPressione ENTER para sair...");
                limparBuffer();
                getchar();
                break;
            default:
                printf("\nOpção inválida!\n");
                printf("Pressione ENTER para continuar...");
                limparBuffer();
                getchar();
        }
    } while (opcao != 5);

    return 0;
}
