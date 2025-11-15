#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif


// 🏰 ESTRUTURAS DE DADOS


typedef struct Sala {
    char nome[50];
    struct Sala *esquerda;
    struct Sala *direita;
} Sala;


// 📋 FUNÇÕES


// 🛠️ FUNÇÕES DE INICIALIZAÇÃO E CONSTRUÇÃO
Sala* criarSala(const char* nome);
Sala* montarMapaMansao();

// 🎮 FUNÇÕES DE INTERAÇÃO E INTERFACE
void limparTela();
void pausarExecucao();
void mostrarTitulo();
void mostrarDespedida();
void mostrarBoasVindas();

// 🔍 FUNÇÕES DE EXPLORAÇÃO E NAVEGAÇÃO
void explorarSalas(Sala* salaAtual);
void processarNavegacao(Sala** salaAtual, char opcao);
void mostrarCaminhosDisponiveis(Sala* salaAtual);
void verificarSalaFinal(Sala* salaAtual);

// 🧹 FUNÇÕES DE FINALIZAÇÃO E MEMÓRIA
void liberarArvore(Sala* raiz);

// =============================================
// 🛠️ FUNÇÕES DE INICIALIZAÇÃO E CONSTRUÇÃO
// =============================================

Sala* criarSala(const char* nome) {
    Sala* novaSala = (Sala*)malloc(sizeof(Sala));
    if (novaSala == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a sala.\n");
        exit(1);
    }
    
    strcpy(novaSala->nome, nome);
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * 🗺️ Monta toda a estrutura do mapa da mansao
 * @return Ponteiro para a raiz da arvore (Hall de Entrada)
 */
Sala* montarMapaMansao() {
    // 🏗️ Criar todas as salas
    Sala* hallEntrada = criarSala("Hall de Entrada");
    Sala* salaEstar = criarSala("Sala de Estar");
    Sala* cozinha = criarSala("Cozinha");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* quartoHospedes = criarSala("Quarto de Hospedes");
    Sala* jardim = criarSala("Jardim");
    Sala* salaJantar = criarSala("Sala de Jantar");
    Sala* escritorio = criarSala("Escritorio Secreto");
    Sala* porao = criarSala("Porao");
    Sala* terraco = criarSala("Terraco");
    Sala* quartoPrincipal = criarSala("Quarto Principal");
    Sala* banheiro = criarSala("Banheiro");
    
    // 🔗 Construir a estrutura (vincula as salas criadas acima)
    hallEntrada->esquerda = salaEstar;
    hallEntrada->direita = cozinha;

    // Nivel 2
    salaEstar->esquerda = biblioteca;
    salaEstar->direita = quartoHospedes;

    cozinha->esquerda = jardim;
    cozinha->direita = salaJantar;

    // Nivel 3
    biblioteca->esquerda = escritorio;
    biblioteca->direita = porao;

    quartoHospedes->esquerda = terraco;
    quartoHospedes->direita = quartoPrincipal;

    jardim->direita = banheiro;

    return hallEntrada;
}

// =============================================
// 🎮 FUNÇÕES DE INTERAÇÃO E INTERFACE
// =============================================

/**
 * 🖥️ Limpa a tela do terminal de forma cross-platform
 */
void limparTela() {
    CLEAR_SCREEN();
}

/**
 * ⏸️ Pausa a execução e espera o usuario pressionar Enter
 */
void pausarExecucao() {
    printf("Pressione Enter para continuar...");
    getchar();
    getchar();
}

/**
 * 🎯 Mostra o titulo do jogo
 */
void mostrarTitulo() {
    printf("=== DETECTIVE QUEST ===\n\n");
}

/**
 * 👋 Mostra mensagem de boas-vindas
 */
void mostrarBoasVindas() {
    printf("Bem-vindo ao Detective Quest!\n");
    printf("Explore a mansao para encontrar pistas e desvendar o misterio.\n");
    printf("Pressione Enter para comecar...");
}

/**
 * 👋 Mostra mensagem de despedida
 */
void mostrarDespedida() {
    limparTela();
    printf("Obrigado por jogar Detective Quest!\n");
    printf("Esperamos que tenha encontrado todas as pistas!\n\n");
}

// =============================================
// 🔍 FUNÇÕES DE EXPLORAÇÃO E NAVEGAÇÃO
// =============================================

/**
 * 🔍 Permite ao jogador explorar as salas da mansao
 * @param salaAtual Ponteiro para a sala atual na exploracao
 */
void explorarSalas(Sala* salaAtual) {
    char opcao;
    
    while (salaAtual != NULL) {
        limparTela();
        mostrarTitulo();
        
        printf("Voce esta na: %s\n\n", salaAtual->nome);
        
        // 🏁 Verifica se e uma sala final
        verificarSalaFinal(salaAtual);
        if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
            break;
        }
        
        // 🧭 Mostra caminhos disponiveis
        mostrarCaminhosDisponiveis(salaAtual);
        
        printf("Para onde deseja ir? ");
        scanf(" %c", &opcao);
        
        // 🎮 Processa a navegacao
        processarNavegacao(&salaAtual, opcao);
    }
}

/**
 * 🧭 Mostra os caminhos disponíveis a partir da sala atual
 * @param salaAtual Ponteiro para a sala atual
 */
void mostrarCaminhosDisponiveis(Sala* salaAtual) {
    printf("Caminhos disponiveis:\n");
    if (salaAtual->esquerda != NULL) {
        printf("[e] Esquerda -> %s\n", salaAtual->esquerda->nome);
    }
    if (salaAtual->direita != NULL) {
        printf("[d] Direita  -> %s\n", salaAtual->direita->nome);
    }
    printf("[s] Sair do jogo\n\n");
}

/**
 * 🏁 Verifica se a sala atual é uma sala final (folha)
 * @param salaAtual Ponteiro para a sala atual
 */
void verificarSalaFinal(Sala* salaAtual) {
    if (salaAtual->esquerda == NULL && salaAtual->direita == NULL) {
        printf(">>> Esta e uma sala final!\n");
        printf(">>> Nao ha mais caminhos para explorar.\n\n");
        pausarExecucao();
    }
}

/**
 * 🎮 Processa a navegação baseada na opção do usuário
 * @param salaAtual Ponteiro para o ponteiro da sala atual
 * @param opcao Opção de navegação escolhida pelo usuário
 */
void processarNavegacao(Sala** salaAtual, char opcao) {
    switch (opcao) {
        case 'e':
        case 'E':
            if ((*salaAtual)->esquerda != NULL) {
                *salaAtual = (*salaAtual)->esquerda;
            } else {
                printf("\n>>> Nao ha caminho a esquerda!\n");
                pausarExecucao();
            }
            break;
            
        case 'd':
        case 'D':
            if ((*salaAtual)->direita != NULL) {
                *salaAtual = (*salaAtual)->direita;
            } else {
                printf("\n>>> Nao ha caminho a direita!\n");
                pausarExecucao();
            }
            break;
            
        case 's':
        case 'S':
            printf("\n>>> Saindo do Detective Quest...\n");
            *salaAtual = NULL;
            break;
            
        default:
            printf("\n>>> Opcao invalida! Use 'e', 'd' ou 's'.\n");
            pausarExecucao();
            break;
    }
}

void liberarArvore(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvore(raiz->esquerda);
    liberarArvore(raiz->direita);
    free(raiz);
}

// FUNÇÃO PRINCIPAL

int main() {
    // 🏗️ Montar o mapa da mansao
    Sala* hallEntrada = montarMapaMansao();
    
    // 🎪 Tela de boas-vindas
    mostrarBoasVindas();
    getchar();
    
    // 🔍 Iniciar exploracao
    explorarSalas(hallEntrada);
    
    // 👋 Mensagem final
    mostrarDespedida();
    
    // 🧹 Liberar memoria
    liberarArvore(hallEntrada);
    
    return 0;
}