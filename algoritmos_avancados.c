#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

// =============================================
// ESTRUTURAS DE DADOS
// =============================================

// Estrutura para os comodos da mansao 
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;

// Estrutura para as pistas coletadas (Arvore BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// =============================================
// PROTOTIPOS DE FUNCOES
// =============================================

// FUNCOES DE INICIALIZACAO E CONSTRUCAO DO MAPA
Sala* criarSala(const char* nome, const char* pista);
Sala* montarMapaMansao();
void construirEstruturaMapa(Sala* hallEntrada, Sala* salaEstar, Sala* cozinha, Sala* biblioteca, 
                           Sala* quartoHospedes, Sala* jardim, Sala* salaJantar, Sala* escritorio, 
                           Sala* porao, Sala* terraco, Sala* quartoPrincipal, Sala* banheiro);

// FUNCOES DA ARVORE DE PISTAS (BST)
PistaNode* criarNoPista(const char* pista);
PistaNode* inserirPista(PistaNode* raiz, const char* pista);
void exibirPistasEmOrdem(PistaNode* raiz);
void liberarArvorePistas(PistaNode* raiz);

// FUNCOES DE INTERACAO E INTERFACE DO USUARIO
void limparTela();
void pausarExecucao();
void mostrarTitulo();
void mostrarBoasVindas();
void mostrarDespedida();
void mostrarStatusInvestigacao(PistaNode* raizPistas);

// FUNCOES DE EXPLORACAO E COLETA DE PISTAS
void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas);
void processarNavegacaoComPistas(Sala** salaAtual, PistaNode** raizPistas, char opcao);
void mostrarCaminhosDisponiveis(Sala* salaAtual);
void coletarPista(Sala* salaAtual, PistaNode** raizPistas);
void verificarPistaColetada(Sala* salaAtual);

// FUNCOES DE FINALIZACAO E GERENCIAMENTO DE MEMORIA
void liberarArvoreSalas(Sala* raiz);
void finalizarJogo(Sala* hallEntrada, PistaNode* raizPistas);

// FUNCOES DE RELATORIO E ESTATISTICAS
void gerarRelatorioFinal(PistaNode* raizPistas);
int contarTotalPistas(PistaNode* raiz);

// =============================================
// FUNCOES DE INICIALIZACAO E CONSTRUCAO DO MAPA
// =============================================

Sala* criarSala(const char* nome, const char* pista) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a sala.\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    strcpy(novaSala->pista, pista);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

Sala* montarMapaMansao() {
    // Criar todas as salas com suas pistas
    Sala* hallEntrada = criarSala("Hall de Entrada", "Porta principal arrombada");
    Sala* salaEstar = criarSala("Sala de Estar", "Copo de vinho pela metade");
    Sala* cozinha = criarSala("Cozinha", "Faca desaparecida do bloco");
    Sala* biblioteca = criarSala("Biblioteca", "Livro sobre venenos aberto");
    Sala* quartoHospedes = criarSala("Quarto de Hospedes", "Mala com documentos secretos");
    Sala* jardim = criarSala("Jardim", "Pegadas de barro frescas");
    Sala* salaJantar = criarSala("Sala de Jantar", "Cadeira desencaixada da mesa");
    Sala* escritorio = criarSala("Escritorio Secreto", "Cofre aberto e vazio");
    Sala* porao = criarSala("Porao", "Manchas escuras no chao");
    Sala* terraco = criarSala("Terraco", "Cinzas de cigarro raro");
    Sala* quartoPrincipal = criarSala("Quarto Principal", "Relogio parado as 23:45");
    Sala* banheiro = criarSala("Banheiro", "Toalha com manchas vermelhas");
    
    // Construir a estrutura usando as salas ja criadas
    construirEstruturaMapa(hallEntrada, salaEstar, cozinha, biblioteca, quartoHospedes, 
                          jardim, salaJantar, escritorio, porao, terraco, quartoPrincipal, banheiro);
    
    return hallEntrada;
}

void construirEstruturaMapa(Sala* hallEntrada, Sala* salaEstar, Sala* cozinha, Sala* biblioteca, 
                           Sala* quartoHospedes, Sala* jardim, Sala* salaJantar, Sala* escritorio, 
                           Sala* porao, Sala* terraco, Sala* quartoPrincipal, Sala* banheiro) {
    // Usar as salas ja criadas em vez de criar novas
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;
    
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quartoHospedes;
    
    cozinha->esquerda = jardim;
    cozinha->direita = salaJantar;
    
    biblioteca->esquerda = escritorio;
    biblioteca->direita = porao;
    
    quartoHospedes->esquerda = terraco;
    quartoHospedes->direita = quartoPrincipal;
    
    jardim->direita = banheiro;
}

// =============================================
// FUNCOES DA ARVORE DE PISTAS (BST)
// =============================================

PistaNode* criarNoPista(const char* pista) {
    PistaNode* novoNo = (PistaNode*)malloc(sizeof(PistaNode));
    if (novoNo == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a pista.\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    
    return novoNo;
}

PistaNode* inserirPista(PistaNode* raiz, const char* pista) {
    if (raiz == NULL) {
        return criarNoPista(pista);
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    
    return raiz;
}

void exibirPistasEmOrdem(PistaNode* raiz) {
    if (raiz != NULL) {
        exibirPistasEmOrdem(raiz->esquerda);
        printf("- %s\n", raiz->pista);
        exibirPistasEmOrdem(raiz->direita);
    }
}

void liberarArvorePistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvorePistas(raiz->esquerda);
    liberarArvorePistas(raiz->direita);
    free(raiz);
}

// =============================================
// FUNCOES DE INTERACAO E INTERFACE DO USUARIO
// =============================================

void limparTela() {
    CLEAR_SCREEN();
}

void pausarExecucao() {
    printf("\nPressione Enter para continuar...");
    getchar();
    getchar();
}

void mostrarTitulo() {
    printf("=== DETECTIVE QUEST - SISTEMA DE PISTAS ===\n\n");
}

void mostrarBoasVindas() {
    printf("Bem-vindo ao Detective Quest - Sistema de Pistas!\n");
    printf("Explore a mansao, colete pistas e resolva o misterio.\n");
    printf("\n");
    printf("Pressione Enter para comecar a investigacao...");
}

void mostrarDespedida() {
    limparTela();
    printf("=== INVESTIGACAO CONCLUIDA ===\n\n");
    printf("Obrigado por jogar Detective Quest!\n");
    printf("Aqui estao todas as pistas coletadas:\n\n");
}

void mostrarStatusInvestigacao(PistaNode* raizPistas) {
    printf("\nStatus da Investigacao:\n");
    printf("Pistas coletadas: %d\n", contarTotalPistas(raizPistas));
}

// =============================================
// FUNCOES DE EXPLORACAO E COLETA DE PISTAS
// =============================================

void coletarPista(Sala* salaAtual, PistaNode** raizPistas) {
    if (strlen(salaAtual->pista) > 0) {
        printf("\n========================================\n");
        printf("PISTA ENCONTRADA NA SALA: %s\n", salaAtual->nome);
        printf("Pista: %s\n", salaAtual->pista);
        printf("========================================\n");
        *raizPistas = inserirPista(*raizPistas, salaAtual->pista);
        printf(">>> Pista registrada no caderno de investigacao!\n");
        strcpy(salaAtual->pista, "");
    } else {
        printf("Nenhuma pista nova encontrada aqui.\n");
    }
}

void verificarPistaColetada(Sala* salaAtual) {
    if (strlen(salaAtual->pista) == 0) {
        printf("Esta sala ja foi investigada.\n");
    }
}

void mostrarCaminhosDisponiveis(Sala* salaAtual) {
    printf("\nCaminhos disponiveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("[e] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("[d] Direita  -> %s\n", salaAtual->direita->nome);
    }
    printf("[s] Sair da investigacao\n");
}

void processarNavegacaoComPistas(Sala** salaAtual, PistaNode** raizPistas, char opcao) {
    switch (opcao) {
        case 'e':
        case 'E':
            if ((*salaAtual)->esquerda != NULL) {
                *salaAtual = (*salaAtual)->esquerda;
                printf("\n>>> Voce entrou em: %s\n", (*salaAtual)->nome);
                if (strlen((*salaAtual)->pista) > 0) {
                    printf(">>> Procurando por pistas...\n");
                }
                coletarPista(*salaAtual, raizPistas);
                pausarExecucao();
            } else {
                printf("\nNao ha caminho a esquerda!\n");
                pausarExecucao();
            }
            break;
            
        case 'd':
        case 'D':
            if ((*salaAtual)->direita != NULL) {
                *salaAtual = (*salaAtual)->direita;
                printf("\n>>> Voce entrou em: %s\n", (*salaAtual)->nome);
                if (strlen((*salaAtual)->pista) > 0) {
                    printf(">>> Procurando por pistas...\n");
                }
                coletarPista(*salaAtual, raizPistas);
                pausarExecucao();
            } else {
                printf("\nNao ha caminho a direita!\n");
                pausarExecucao();
            }
            break;
            
        case 's':
        case 'S':
            printf("\nSaindo do Detective Quest...\n");
            *salaAtual = NULL;
            break;
            
        default:
            printf("\nOpcao invalida! Use 'e', 'd' ou 's'.\n");
            pausarExecucao();
            break;
    }
}

void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas) {
    char opcao;
    
    // Coletar pista apenas do Hall de Entrada no inicio
    printf("\n>>> Voce comecou no: %s\n", salaAtual->nome);
    if (strlen(salaAtual->pista) > 0) {
        printf(">>> Procurando por pistas...\n");
    }
    coletarPista(salaAtual, raizPistas);
    pausarExecucao();
    
    while (salaAtual != NULL) {
        limparTela();
        mostrarTitulo();
        
        printf("Voce esta na: %s\n", salaAtual->nome);
        verificarPistaColetada(salaAtual);
        mostrarStatusInvestigacao(*raizPistas);
        
        mostrarCaminhosDisponiveis(salaAtual);
        
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        processarNavegacaoComPistas(&salaAtual, raizPistas, opcao);
    }
}

// =============================================
// FUNCOES DE FINALIZACAO E GERENCIAMENTO DE MEMORIA
// =============================================

void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

void finalizarJogo(Sala* hallEntrada, PistaNode* raizPistas) {
    gerarRelatorioFinal(raizPistas);
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
}

// =============================================
// FUNCOES DE RELATORIO E ESTATISTICAS
// =============================================

void gerarRelatorioFinal(PistaNode* raizPistas) {
    mostrarDespedida();
    exibirPistasEmOrdem(raizPistas);
    printf("\nTotal de pistas coletadas: %d\n", contarTotalPistas(raizPistas));
    printf("Investigacao encerrada. Volte sempre, detetive!\n");
}

int contarTotalPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarTotalPistas(raiz->esquerda) + contarTotalPistas(raiz->direita);
}

// =============================================
// FUNCAO PRINCIPAL
// =============================================

int main() {
    // Montar o mapa da mansao
    Sala* hallEntrada = montarMapaMansao();
    
    // Inicializar arvore de pistas (vazia)
    PistaNode* raizPistas = NULL;
    
    // Tela de boas-vindas
    mostrarBoasVindas();
    getchar();
    
    // Iniciar exploracao com coleta de pistas
    explorarSalasComPistas(hallEntrada, &raizPistas);
    
    // Finalizar jogo com relatorio
    finalizarJogo(hallEntrada, raizPistas);
    
    return 0;
}