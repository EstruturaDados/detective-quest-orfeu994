#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN() system("cls")
#else
    #define CLEAR_SCREEN() system("clear")
#endif

// --- Estruturas de dados
// ---

// Sala (comodo)
typedef struct Sala {
    char nome[50];
    char pista[100];
    struct Sala *esquerda;
    struct Sala *direita;
    int visitada; // marcar se a sala foi visitada
} Sala;

// Pista (BST)
typedef struct PistaNode {
    char pista[100];
    struct PistaNode *esquerda;
    struct PistaNode *direita;
} PistaNode;

// Hash (pista -> suspeito)
typedef struct HashNode {
    char pista[100];
    char suspeito[50];
    struct HashNode *proximo;
} HashNode;

// Constantes
#define TAMANHO_HASH 10

// Protótipos

// Mapa
Sala* criarSala(const char* nome, const char* pista);
Sala* montarMapaMansao();
void construirEstruturaMapa(Sala* hallEntrada, Sala* salaEstar, Sala* cozinha, Sala* biblioteca, 
                           Sala* quartoHospedes, Sala* jardim, Sala* salaJantar, Sala* escritorio, 
                           Sala* porao, Sala* terraco, Sala* quartoPrincipal, Sala* banheiro);

// Pistas (BST)
PistaNode* criarNoPista(const char* pista);
PistaNode* inserirPista(PistaNode* raiz, const char* pista);
void exibirPistasEmOrdem(PistaNode* raiz);
void liberarArvorePistas(PistaNode* raiz);

// Hash
HashNode** criarTabelaHash(int tamanho);
int calcularHash(const char* pista, int tamanho);
void inserirNaHash(HashNode** tabela, int tamanho, const char* pista, const char* suspeito);
char* encontrarSuspeito(HashNode** tabela, int tamanho, const char* pista);
void inicializarTabelaHash(HashNode** tabela, int tamanho);
void liberarTabelaHash(HashNode** tabela, int tamanho);
void revelarCulpadoReal();

// Interface
void limparTela();
void pausarExecucao();
void mostrarTitulo();
void mostrarBoasVindas();
void mostrarDespedida();
void mostrarStatusInvestigacao(PistaNode* raizPistas);

// Navegação
void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash, Sala* hallEntrada);
void processarNavegacaoComPistas(Sala** salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash, char opcao, Sala* hallEntrada);
void mostrarCaminhosDisponiveis(Sala* salaAtual);
int verificarFimDoCaminho(Sala* salaAtual);
void coletarPista(Sala* salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash);
void verificarPistaColetada(Sala* salaAtual);
void marcarSalaComoVisitada(Sala* salaAtual);
void resetarVisitas(Sala* raiz);
int todasSalasVisitadas(Sala* raiz);

// Julgamento final
void verificarSuspeitoFinal(PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash);
int contarPistasPorSuspeito(PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash, const char* suspeito);
void listarSuspeitos();

// Relatórios e estatísticas
void gerarRelatorioFinal(PistaNode* raizPistas);
int contarTotalPistas(PistaNode* raiz);

// Finalização e memória
void liberarArvoreSalas(Sala* raiz);
void finalizarJogo(Sala* hallEntrada, PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash);

// Implementação

// --- Mapa

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
    novaSala->visitada = 0; // Inicialmente não visitada
    
    return novaSala;
}

Sala* montarMapaMansao() {
    struct {
        const char* nome;
        const char* pista;
    } salas_dados[] = {
        {"Hall de Entrada", "Porta principal arrombada"},
        {"Sala de Estar", "Copo de vinho pela metade"},
        {"Cozinha", "Faca desaparecida do bloco"},
        {"Biblioteca", "Livro sobre venenos aberto"},
        {"Quarto de Hospedes", "Mala com documentos secretos"},
        {"Jardim", "Pegadas de barro frescas"},
        {"Sala de Jantar", "Cadeira desencaixada da mesa"},
        {"Escritorio Secreto", "Cofre aberto e vazio"},
        {"Porao", "Manchas escuras no chao"},
        {"Terraco", "Cinzas de cigarro raro"},
        {"Quarto Principal", "Relogio parado as 23:45"},
        {"Banheiro", "Toalha com manchas vermelhas"}
    };

    const int NUM_SALAS = sizeof(salas_dados) / sizeof(salas_dados[0]);
    Sala* salas[NUM_SALAS];

    for (int i = 0; i < NUM_SALAS; i++) {
        salas[i] = criarSala(salas_dados[i].nome, salas_dados[i].pista);
    }

    construirEstruturaMapa(salas[0], salas[1], salas[2], salas[3], salas[4],
                          salas[5], salas[6], salas[7], salas[8], salas[9], salas[10], salas[11]);

    return salas[0];
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

// --- Pistas (BST)

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

// --- Hash

HashNode** criarTabelaHash(int tamanho) {
    HashNode** tabela = (HashNode**)malloc(tamanho * sizeof(HashNode*));
    if (tabela == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para a tabela hash.\n");
        exit(1);
    }
    
    for (int i = 0; i < tamanho; i++) {
        tabela[i] = NULL;
    }
    
    return tabela;
}

int calcularHash(const char* pista, int tamanho) {
    int hash = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        hash = (hash * 31 + pista[i]) % tamanho;
    }
    return hash;
}

void inserirNaHash(HashNode** tabela, int tamanho, const char* pista, const char* suspeito) {
    int indice = calcularHash(pista, tamanho);
    
    HashNode* novoNo = (HashNode*)malloc(sizeof(HashNode));
    if (novoNo == NULL) {
        printf("Erro: Nao foi possivel alocar memoria para o no hash.\n");
        exit(1);
    }
    
    strcpy(novoNo->pista, pista);
    strcpy(novoNo->suspeito, suspeito);
    novoNo->proximo = tabela[indice];
    tabela[indice] = novoNo;
}

char* encontrarSuspeito(HashNode** tabela, int tamanho, const char* pista) {
    int indice = calcularHash(pista, tamanho);
    HashNode* atual = tabela[indice];
    
    while (atual != NULL) {
        if (strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->proximo;
    }
    
    return "Desconhecido";
}

void inicializarTabelaHash(HashNode** tabela, int tamanho) {
    
    // Joao
    inserirNaHash(tabela, tamanho, "Porta principal arrombada", "Joao");
    inserirNaHash(tabela, tamanho, "Pegadas de barro frescas", "Joao");
    inserirNaHash(tabela, tamanho, "Relogio parado as 23:45", "Joao");
    inserirNaHash(tabela, tamanho, "Cartao de acesso roubado", "Joao");
    inserirNaHash(tabela, tamanho, "Dinheiro em especie escondido", "Joao");

    // Maria
    inserirNaHash(tabela, tamanho, "Copo de vinho pela metade", "Maria");
    inserirNaHash(tabela, tamanho, "Cadeira desencaixada da mesa", "Maria");
    inserirNaHash(tabela, tamanho, "Toalha com manchas vermelhas", "Maria");
    inserirNaHash(tabela, tamanho, "Luvas de latex encontradas", "Maria");
    inserirNaHash(tabela, tamanho, "Celular com ligacoes suspeitas", "Maria");

    // Carlos
    inserirNaHash(tabela, tamanho, "Faca desaparecida do bloco", "Carlos");
    inserirNaHash(tabela, tamanho, "Cofre aberto e vazio", "Carlos");
    inserirNaHash(tabela, tamanho, "Computador com emails suspeitos", "Carlos");
    inserirNaHash(tabela, tamanho, "Documentos falsificados", "Carlos");

    // Ana
    inserirNaHash(tabela, tamanho, "Livro sobre venenos aberto", "Ana");
    inserirNaHash(tabela, tamanho, "Manchas escuras no chao", "Ana");
    inserirNaHash(tabela, tamanho, "Fotos comprometedoras", "Ana");
    inserirNaHash(tabela, tamanho, "Chave mestra duplicada", "Ana");

    // Pedro
    inserirNaHash(tabela, tamanho, "Mala com documentos secretos", "Pedro");
    inserirNaHash(tabela, tamanho, "Cinzas de cigarro raro", "Pedro");
    inserirNaHash(tabela, tamanho, "Gravação de segurança apagada", "Pedro");
    inserirNaHash(tabela, tamanho, "Horario alterado no sistema", "Pedro");
}

void liberarTabelaHash(HashNode** tabela, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        HashNode* atual = tabela[i];
        while (atual != NULL) {
            HashNode* proximo = atual->proximo;
            free(atual);
            atual = proximo;
        }
    }
    free(tabela);
}

void revelarCulpadoReal() {
    printf("\n*** O VERDADEIRO CULPADO ERA: JOAO ***\n");
    printf("Ele tinha 4 pistas incriminadoras:\n");
    printf("- Porta principal arrombada\n");
    printf("- Pegadas de barro frescas\n");
    printf("- Relogio parado as 23:45\n");
    printf("- Cartao de acesso roubado\n");
}

// --- Interface

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

// --- Navegação

void marcarSalaComoVisitada(Sala* salaAtual) {
    if (salaAtual != NULL) {
        salaAtual->visitada = 1;
    }
}

void resetarVisitas(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    raiz->visitada = 0;
    resetarVisitas(raiz->esquerda);
    resetarVisitas(raiz->direita);
}

int todasSalasVisitadas(Sala* raiz) {
    if (raiz == NULL) {
        return 1;
    }
    if (!raiz->visitada) {
        return 0;
    }
    return todasSalasVisitadas(raiz->esquerda) && todasSalasVisitadas(raiz->direita);
}

int verificarFimDoCaminho(Sala* salaAtual) {
    return (salaAtual->esquerda == NULL && salaAtual->direita == NULL);
}

void coletarPista(Sala* salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash) {
    if (strlen(salaAtual->pista) > 0) {
        printf("\n========================================\n");
        printf("PISTA ENCONTRADA NA SALA: %s\n", salaAtual->nome);
        printf("Pista: %s\n", salaAtual->pista);
        
        char* suspeito = encontrarSuspeito(tabelaHash, tamanhoHash, salaAtual->pista);
        printf("Suspeito associado: %s\n", suspeito);
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
    if (salaAtual->esquerda != NULL && !salaAtual->esquerda->visitada) {
        printf("[e] Esquerda -> %s (NOVO)\n", salaAtual->esquerda->nome);
    } else if (salaAtual->esquerda != NULL) {
        printf("[e] Esquerda -> %s (VISITADO)\n", salaAtual->esquerda->nome);
    }
    
    if (salaAtual->direita != NULL && !salaAtual->direita->visitada) {
        printf("[d] Direita  -> %s (NOVO)\n", salaAtual->direita->nome);
    } else if (salaAtual->direita != NULL) {
        printf("[d] Direita  -> %s (VISITADO)\n", salaAtual->direita->nome);
    }
    printf("[s] Sair da investigacao\n");
}

void processarNavegacaoComPistas(Sala** salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash, char opcao, Sala* hallEntrada) {
    switch (opcao) {
        case 'e':
        case 'E':
            if ((*salaAtual)->esquerda != NULL) {
                // Verificar se a sala já foi visitada
                if ((*salaAtual)->esquerda->visitada) {
                    printf("\nVoce ja visitou esta sala anteriormente!\n");
                    printf("Deseja visitar novamente? (s/n): ");
                    char resposta;
                    scanf(" %c", &resposta);
                    if (resposta != 's' && resposta != 'S') {
                        printf("Escolha outro caminho.\n");
                        pausarExecucao();
                        break;
                    }
                }
                
                *salaAtual = (*salaAtual)->esquerda;
                marcarSalaComoVisitada(*salaAtual); // Marcar como visitada
                printf("\n>>> Voce entrou em: %s\n", (*salaAtual)->nome);
                if (strlen((*salaAtual)->pista) > 0) {
                    printf(">>> Procurando por pistas...\n");
                }
                coletarPista(*salaAtual, raizPistas, tabelaHash, tamanhoHash);
                
                // Verificar se chegou ao fim do caminho - VOLTA AUTOMATICAMENTE
                if (verificarFimDoCaminho(*salaAtual)) {
                    printf("\n>>> Voce chegou ao fim deste caminho!\n");
                    printf("Voltando automaticamente ao Hall de Entrada...\n");
                    *salaAtual = hallEntrada; // Volta para o Hall de Entrada
                    pausarExecucao();
                } else {
                    pausarExecucao();
                }
            } else {
                printf("\nNao ha caminho a esquerda!\n");
                pausarExecucao();
            }
            break;
            
        case 'd':
        case 'D':
            if ((*salaAtual)->direita != NULL) {
                // Verificar se a sala já foi visitada
                if ((*salaAtual)->direita->visitada) {
                    printf("\nVoce ja visitou esta sala anteriormente!\n");
                    printf("Deseja visitar novamente? (s/n): ");
                    char resposta;
                    scanf(" %c", &resposta);
                    if (resposta != 's' && resposta != 'S') {
                        printf("Escolha outro caminho.\n");
                        pausarExecucao();
                        break;
                    }
                }
                
                *salaAtual = (*salaAtual)->direita;
                marcarSalaComoVisitada(*salaAtual); // Marcar como visitada
                printf("\n>>> Voce entrou em: %s\n", (*salaAtual)->nome);
                if (strlen((*salaAtual)->pista) > 0) {
                    printf(">>> Procurando por pistas...\n");
                }
                coletarPista(*salaAtual, raizPistas, tabelaHash, tamanhoHash);
                
                // Verificar se chegou ao fim do caminho - VOLTA AUTOMATICAMENTE
                if (verificarFimDoCaminho(*salaAtual)) {
                    printf("\n>>> Voce chegou ao fim deste caminho!\n");
                    printf("Voltando automaticamente ao Hall de Entrada...\n");
                    *salaAtual = hallEntrada; // Volta para o Hall de Entrada
                    pausarExecucao();
                } else {
                    pausarExecucao();
                }
            } else {
                printf("\nNao ha caminho a direita!\n");
                pausarExecucao();
            }
            break;
            
        case 's':
        case 'S':
            printf("\nDeseja realmente sair da investigacao? (s/n): ");
            char confirmacao;
            scanf(" %c", &confirmacao);
            if (confirmacao == 's' || confirmacao == 'S') {
                printf("Saindo do Detective Quest...\n");
                *salaAtual = NULL;
            }
            break;
            
        default:
            printf("\nOpcao invalida! Use 'e', 'd' ou 's'.\n");
            pausarExecucao();
            break;
    }
}

void explorarSalasComPistas(Sala* salaAtual, PistaNode** raizPistas, HashNode** tabelaHash, int tamanhoHash, Sala* hallEntrada) {
    char opcao;
    
    // Marcar Hall de Entrada como visitada no início
    marcarSalaComoVisitada(salaAtual);
    
    // Coletar pista apenas do Hall de Entrada no inicio
    printf("\n>>> Voce comecou no: %s\n", salaAtual->nome);
    if (strlen(salaAtual->pista) > 0) {
        printf(">>> Procurando por pistas...\n");
    }
    coletarPista(salaAtual, raizPistas, tabelaHash, tamanhoHash);
    pausarExecucao();
    
    while (salaAtual != NULL) {
        limparTela();
        mostrarTitulo();
        
        printf("Voce esta na: %s\n", salaAtual->nome);
        verificarPistaColetada(salaAtual);
        mostrarStatusInvestigacao(*raizPistas);
        
        // Verificar se todas as salas foram visitadas
        if (todasSalasVisitadas(hallEntrada)) {
            printf("\n>>> PARABENS! Voce explorou todas as salas da mansao!\n");
            printf("Voce pode continuar explorando ou sair para o julgamento final.\n");
        }
        
        mostrarCaminhosDisponiveis(salaAtual);
        
        printf("\nPara onde deseja ir? ");
        scanf(" %c", &opcao);
        
        processarNavegacaoComPistas(&salaAtual, raizPistas, tabelaHash, tamanhoHash, opcao, hallEntrada);
    }
}

// --- Suspeitos

void listarSuspeitos() {
    printf("\n=== SUSPEITOS ===\n");
    printf("1. Joao\n");
    printf("2. Maria\n");
    printf("3. Carlos\n");
    printf("4. Ana\n");
    printf("5. Pedro\n");
    printf("=================\n");
}

int contarPistasPorSuspeito(PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash, const char* suspeito) {
    if (raizPistas == NULL) {
        return 0;
    }
    
    int contador = 0;
    
    // Contar pistas da subarvore esquerda
    contador += contarPistasPorSuspeito(raizPistas->esquerda, tabelaHash, tamanhoHash, suspeito);
    
    // Verificar pista atual
    char* suspeitoDaPista = encontrarSuspeito(tabelaHash, tamanhoHash, raizPistas->pista);
    if (strcmp(suspeitoDaPista, suspeito) == 0) {
        contador++;
    }
    
    // Contar pistas da subarvore direita
    contador += contarPistasPorSuspeito(raizPistas->direita, tabelaHash, tamanhoHash, suspeito);
    
    return contador;
}

void verificarSuspeitoFinal(PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash) {
    char suspeito[50];
    
    printf("\n=== FASE DE JULGAMENTO ===\n");
    printf("Com base nas pistas coletadas, quem e o culpado?\n");
    
    listarSuspeitos();
    
    printf("\nDigite o nome do suspeito: ");
    scanf(" %[^\n]", suspeito);
    
    int pistasDoSuspeito = contarPistasPorSuspeito(raizPistas, tabelaHash, tamanhoHash, suspeito);
    
    printf("\n=== RESULTADO DO JULGAMENTO ===\n");
    printf("Suspeito acusado: %s\n", suspeito);
    printf("Pistas que apontam para %s: %d\n", suspeito, pistasDoSuspeito);
    
    if (pistasDoSuspeito >= 2) {
        printf("\n*** PARABENS! ***\n");
        printf("Voce reuniu provas suficientes para incriminar %s!\n", suspeito);
        printf("O caso esta resolvido!\n");
    } else {
        printf("\n*** ATENCAO! ***\n");
        printf("Poucas pistas apontam para %s.\n", suspeito);
        printf("Voce nao tem provas suficientes para uma acusacao.\n");
        printf("O verdadeiro culpado pode escapar!\n");
    }
}

void gerarRelatorioFinal(PistaNode* raizPistas) {
    mostrarDespedida();
    exibirPistasEmOrdem(raizPistas);
    printf("\nTotal de pistas coletadas: %d\n", contarTotalPistas(raizPistas));
}

int contarTotalPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return 0;
    }
    return 1 + contarTotalPistas(raiz->esquerda) + contarTotalPistas(raiz->direita);
}

// --- Finalização

void liberarArvoreSalas(Sala* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    liberarArvoreSalas(raiz->esquerda);
    liberarArvoreSalas(raiz->direita);
    free(raiz);
}

void finalizarJogo(Sala* hallEntrada, PistaNode* raizPistas, HashNode** tabelaHash, int tamanhoHash) {
    gerarRelatorioFinal(raizPistas);
    verificarSuspeitoFinal(raizPistas, tabelaHash, tamanhoHash);
    
    // Revelar o culpado real se o jogador nao acertou
    printf("\nDeseja saber quem era o verdadeiro culpado? (s/n): ");
    char resposta;
    scanf(" %c", &resposta);
    if (resposta == 's' || resposta == 'S') {
        revelarCulpadoReal();
    }
    
    liberarArvoreSalas(hallEntrada);
    liberarArvorePistas(raizPistas);
    liberarTabelaHash(tabelaHash, tamanhoHash);
}

// --- Principal

int main() {
    Sala* hallEntrada = montarMapaMansao();
    PistaNode* raizPistas = NULL;
    HashNode** tabelaHash = criarTabelaHash(TAMANHO_HASH);
    inicializarTabelaHash(tabelaHash, TAMANHO_HASH);

    mostrarBoasVindas();
    getchar();

    explorarSalasComPistas(hallEntrada, &raizPistas, tabelaHash, TAMANHO_HASH, hallEntrada);
    finalizarJogo(hallEntrada, raizPistas, tabelaHash, TAMANHO_HASH);

    return 0;
}