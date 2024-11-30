#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>

// Estrutura para o nodo da ABP
struct nodo {
    char info[100];
    struct nodo *esq;
    struct nodo *dir;
    int quantidade;
};
typedef struct nodo NODO;

// Estrutura para a lista de letras
struct lista_letras {
    char info;
    struct lista_letras *prox;
    NODO *nodo;
};
typedef struct lista_letras LISTA_LETRAS;

LISTA_LETRAS *l = NULL;

// Inicializa a lista de letras
void inicializa_lista_letras() {
    LISTA_LETRAS *novo, *aux = NULL;
    char letra;

    for (letra = 'A'; letra <= 'Z'; letra++) {
        novo = (LISTA_LETRAS *)malloc(sizeof(LISTA_LETRAS));
        if (novo == NULL) {
            printf("Erro de alocação de memória.\n");
            exit(1);
        }
        novo->nodo = NULL;
        novo->info = letra;
        novo->prox = NULL;
        if (l == NULL) {
            l = novo;
        } else {
            aux->prox = novo;
        }
        aux = novo;
    }
}

// Insere uma palavra na ABP
void insere_palavra() {
    LISTA_LETRAS *aux = l;
    char palavra[100];
    printf("Digite a palavra que deseja armazenar: ");
    scanf("%s", palavra);

    while (aux != NULL && tolower(aux->info) != tolower(palavra[0])) {
        aux = aux->prox;
    }

    if (aux == NULL) {
        printf("Letra inicial não encontrada.\n");
        return;
    }

    NODO **raiz = &aux->nodo;
    while (*raiz != NULL) {
        int cmp = strcasecmp((*raiz)->info, palavra);
        if (cmp == 0) {
            (*raiz)->quantidade++;
            printf("Palavra já existente, quantidade incrementada.\n");
            return;
        } else if (cmp > 0) {
            raiz = &(*raiz)->esq;
        } else {
            raiz = &(*raiz)->dir;
        }
    }

    *raiz = (NODO *)malloc(sizeof(NODO));
    if (*raiz == NULL) {
        printf("Erro de alocação de memória.\n");
        exit(1);
    }
    strcpy((*raiz)->info, palavra);
    (*raiz)->esq = (*raiz)->dir = NULL;
    (*raiz)->quantidade = 1;
    printf("Palavra inserida com sucesso.\n");
}

// Consulta uma palavra
void consulta_palavra() {
    LISTA_LETRAS *aux = l;
    char palavra[100];
    printf("Digite a palavra que deseja consultar: ");
    scanf("%s", palavra);

    while (aux != NULL && tolower(aux->info) != tolower(palavra[0])) {
        aux = aux->prox;
    }

    if (aux == NULL || aux->nodo == NULL) {
        printf("Palavra não encontrada.\n");
        return;
    }

    NODO *raiz = aux->nodo;
    while (raiz != NULL) {
        int cmp = strcasecmp(raiz->info, palavra);
        if (cmp == 0) {
            printf("Palavra encontrada! Ocorrências: %d\n", raiz->quantidade);
            return;
        } else if (cmp > 0) {
            raiz = raiz->esq;
        } else {
            raiz = raiz->dir;
        }
    }
    printf("Palavra não encontrada.\n");
}

// Contagem de palavras e ocorrências
int conta_palavras_rec(NODO *raiz) {
    if (raiz == NULL) return 0;
    return 1 + conta_palavras_rec(raiz->esq) + conta_palavras_rec(raiz->dir);
}

int conta_ocorrencias_rec(NODO *raiz) {
    if (raiz == NULL) return 0;
    return raiz->quantidade + conta_ocorrencias_rec(raiz->esq) + conta_ocorrencias_rec(raiz->dir);
}

// Conta o número total de palavras
void conta_palavras() {
    LISTA_LETRAS *aux = l;
    int total = 0;

    while (aux != NULL) {
        total += conta_palavras_rec(aux->nodo);
        aux = aux->prox;
    }

    printf("Número total de palavras: %d\n", total);
}

// Conta o número total de ocorrências
void conta_ocorrencias() {
    LISTA_LETRAS *aux = l;
    int total = 0;

    while (aux != NULL) {
        total += conta_ocorrencias_rec(aux->nodo);
        aux = aux->prox;
    }

    printf("Número total de ocorrências: %d\n", total);
}

// Exibe palavras em ordem alfabética
void exibe_palavras_rec(NODO *raiz) {
    if (raiz == NULL) return;
    exibe_palavras_rec(raiz->esq);
    printf("%s (%d)\n", raiz->info, raiz->quantidade);
    exibe_palavras_rec(raiz->dir);
}

void exibe_palavras() {
    LISTA_LETRAS *aux = l;
    while (aux != NULL) {
        printf("Palavras começando com '%c':\n", aux->info);
        exibe_palavras_rec(aux->nodo);
        aux = aux->prox;
    }
}

// Remover uma palavra
NODO *remove_nodo(NODO *raiz, char *palavra, int *removido) {
    if (raiz == NULL) return NULL;

    int cmp = strcasecmp(raiz->info, palavra);
    if (cmp > 0) {
        raiz->esq = remove_nodo(raiz->esq, palavra, removido);
    } else if (cmp < 0) {
        raiz->dir = remove_nodo(raiz->dir, palavra, removido);
    } else {
        *removido = 1;
        if (raiz->esq == NULL) {
            NODO *temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            NODO *temp = raiz->esq;
            free(raiz);
            return temp;
        } else {
            NODO *temp = raiz->dir;
            while (temp->esq != NULL) {
                temp = temp->esq;
            }
            strcpy(raiz->info, temp->info);
            raiz->quantidade = temp->quantidade;
            raiz->dir = remove_nodo(raiz->dir, temp->info, removido);
        }
    }

    return raiz;
}

void remove_palavra() {
    LISTA_LETRAS *aux = l;
    char palavra[100];
    printf("Digite a palavra que deseja remover: ");
    scanf("%s", palavra);

    while (aux != NULL && tolower(aux->info) != tolower(palavra[0])) {
        aux = aux->prox;
    }

    if (aux == NULL || aux->nodo == NULL) {
        printf("Palavra não encontrada.\n");
        return;
    }

    int removido = 0;
    aux->nodo = remove_nodo(aux->nodo, palavra, &removido);

    if (removido) {
        printf("Palavra removida com sucesso.\n");
    } else {
        printf("Palavra não encontrada.\n");
    }
}

void remove_ocorrencia() {
    LISTA_LETRAS *aux = l;
    char palavra[100];
    printf("Digite a palavra que deseja remover uma ocorrência: ");
    scanf("%s", palavra);

    while (aux != NULL && tolower(aux->info) != tolower(palavra[0])) {
        aux = aux->prox;
    }

    if (aux == NULL || aux->nodo == NULL) {
        printf("Erro: Letra inicial ou palavra não encontrada.\n");
        return;
    }

    NODO *raiz = aux->nodo, *pai = NULL;
    while (raiz != NULL) {
        int comparacao = strcasecmp(raiz->info, palavra);
        if (comparacao == 0) {
            if (--raiz->quantidade <= 0) {
                raiz = remove_nodo(raiz, palavra, &comparacao);
            }
            printf("Ocorrência removida.\n");
            return;
        }
        pai = raiz;
        raiz = comparacao > 0 ? raiz->esq : raiz->dir;
    }
    printf("Palavra não encontrada.\n");
}

// Função de percurso pré-fixado
void percurso_pre_fixado_rec(NODO *raiz) {
    if (raiz == NULL) return;
    printf("%s (%d)\n", raiz->info, raiz->quantidade);
    percurso_pre_fixado_rec(raiz->esq);
    percurso_pre_fixado_rec(raiz->dir);
}

void percurso_pre_fixado() {
    LISTA_LETRAS *aux = l;
    while (aux != NULL) {
        printf("Palavras começando com '%c' (pré-fixado):\n", aux->info);
        percurso_pre_fixado_rec(aux->nodo);
        aux = aux->prox;
    }
}

// Função de percurso pós-fixado
void percurso_pos_fixado_rec(NODO *raiz) {
    if (raiz == NULL) return;
    percurso_pos_fixado_rec(raiz->esq);
    percurso_pos_fixado_rec(raiz->dir);
    printf("%s (%d)\n", raiz->info, raiz->quantidade);
}

void percurso_pos_fixado() {
    LISTA_LETRAS *aux = l;
    while (aux != NULL) {
        printf("Palavras começando com '%c' (pós-fixado):\n", aux->info);
        percurso_pos_fixado_rec(aux->nodo);
        aux = aux->prox;
    }
}

// Função de percurso central
void percurso_central_rec(NODO *raiz) {
    if (raiz == NULL) return;
    percurso_central_rec(raiz->esq);
    printf("%s (%d)\n", raiz->info, raiz->quantidade);
    percurso_central_rec(raiz->dir);
}

void percurso_central() {
    LISTA_LETRAS *aux = l;
    while (aux != NULL) {
        printf("Palavras começando com '%c' (central):\n", aux->info);
        percurso_central_rec(aux->nodo);
        aux = aux->prox;
    }
}

// Encontra a letra com mais palavras
void letra_com_mais_palavras() {
    LISTA_LETRAS *aux = l;
    int max_palavras = 0;
    char letra_com_mais_palavras = '\0';

    while (aux != NULL) {
        int num_palavras = conta_palavras_rec(aux->nodo);
        if (num_palavras > max_palavras) {
            max_palavras = num_palavras;
            letra_com_mais_palavras = aux->info;
        }
        aux = aux->prox;
    }

    if (letra_com_mais_palavras != '\0') {
        printf("Letra com mais palavras: '%c' (Número de palavras: %d)\n", 
               letra_com_mais_palavras, max_palavras);
    } else {
        printf("Não há palavras registradas.\n");
    }
}

// Encontra a palavra com mais ocorrências
void palavra_com_mais_ocorrencias() {
    LISTA_LETRAS *aux = l;
    NODO *nodo_com_mais_ocorrencias = NULL;
    int max_ocorrencias = 0;

    while (aux != NULL) {
        NODO *raiz = aux->nodo;
        while (raiz != NULL) {
            if (raiz->quantidade > max_ocorrencias) {
                max_ocorrencias = raiz->quantidade;
                nodo_com_mais_ocorrencias = raiz;
            }
            raiz = raiz->dir;  // Percorre para a direita, já que a árvore está ordenada
        }
        aux = aux->prox;
    }

    if (nodo_com_mais_ocorrencias != NULL) {
        printf("Palavra com mais ocorrências: '%s' (Ocorrências: %d)\n",
               nodo_com_mais_ocorrencias->info, nodo_com_mais_ocorrencias->quantidade);
    } else {
        printf("Não há palavras registradas.\n");
    }
}


// Exibe palavras por letra
void exibe_palavras_por_letra() {
    LISTA_LETRAS *aux = l;
    
    while (aux != NULL) {
        if (aux->nodo != NULL) {
            printf("Palavras começando com '%c':\n", aux->info);
            exibe_palavras_rec(aux->nodo);  // já existente, que exibe em ordem alfabética
        }
        aux = aux->prox;
    }
}


// Função do menu
void menu() {
    int opcao;
    inicializa_lista_letras();

    do {
        printf("\nMenu:\n");
        printf("1. Inserir palavra\n");
        printf("2. Consultar palavra\n");
        printf("3. Contar palavras\n");
        printf("4. Contar ocorrências\n");
        printf("5. Exibir palavras\n");
        printf("6. Remover palavra\n");
        printf("7. Remover ocorrência\n");
        printf("8. Percurso pré-fixado\n");
        printf("9. Percurso pós-fixado\n");
        printf("10. Percurso central\n");
        printf("11. Exibir palavras por letra\n");
        printf("12. Palavra com mais ocorrências\n");
        printf("13. Letra com mais palavras\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: insere_palavra(); break;
            case 2: consulta_palavra(); break;
            case 3: conta_palavras(); break;
            case 4: conta_ocorrencias(); break;
            case 5: exibe_palavras(); break;
            case 6: remove_palavra(); break;
            case 7: remove_ocorrencia(); break;
            case 8: percurso_pre_fixado(); break;
            case 9: percurso_pos_fixado(); break;
            case 10: percurso_central(); break;
            case 11: exibe_palavras_por_letra(); break;
            case 12: palavra_com_mais_ocorrencias(); break;
            case 13: letra_com_mais_palavras(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida.\n"); break;
        }
    } while (opcao != 0);
}


int main() {
	setlocale(LC_ALL, "Portuguese");
    menu();
    return 0;
}

