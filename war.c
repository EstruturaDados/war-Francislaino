// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.
#define TAM_NOME 30
#define TAM_COR 10

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} territorio;

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.

// Funções de setup e gerenciamento de memória:
territorio* alocarMapa(int quantidade);
void liberarMemoria(territorio *mapa);

// Funções de interface com o usuário:
void cadastrarTerritorios(territorio *mapa, int quantidade);
void exibirMapa(const territorio *mapa, int quantidade);

// Funções de lógica principal do jogo:
void atacar(territorio *atacante, territorio *defensor);

// Função utilitária:
void limparBuffer();

// --- Implementação da Função Utilitária ---
// Função responsável por limpar o buffer do teclado.
void limparBuffer() {

    int c;

    while ((c = getchar()) != '\n' && c != EOF);
}

// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {

    // 1. Configuração Inicial (Setup):

    int quantidadeTerritorios;
    int atacanteIndex;
    int defensorIndex;
    int opcao;

    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    srand(time(NULL));

    // Solicita ao usuário a quantidade de territórios.
    printf("========================================\n");
    printf("         PROJETO WAR - FASE 2\n");
    printf("========================================\n");

    printf("Digite a quantidade de territorios: ");
    scanf("%d", &quantidadeTerritorios);

    limparBuffer();

    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    territorio *mapa = alocarMapa(quantidadeTerritorios);

    if (mapa == NULL) {

        printf("Erro ao alocar memoria!\n");

        return 1;
    }

    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    cadastrarTerritorios(mapa, quantidadeTerritorios);

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0).
    do {

        // - A cada iteração, exibe o mapa.
        exibirMapa(mapa, quantidadeTerritorios);

        printf("\n========================================\n");
        printf("             FASE DE ATAQUE\n");
        printf("========================================\n");

        printf("\nEscolha o territorio atacante (1 a %d): ",
       quantidadeTerritorios);

        scanf("%d", &atacanteIndex);

        printf("Escolha o territorio defensor (1 a %d): ",
            quantidadeTerritorios);

        scanf("%d", &defensorIndex);
        

        // Ajuste para índice do vetor
        atacanteIndex--;
        defensorIndex--;

        // Realiza validações antes do ataque.

        // Verifica se os índices são válidos.
        if (atacanteIndex < 0 || atacanteIndex >= quantidadeTerritorios ||
            defensorIndex < 0 || defensorIndex >= quantidadeTerritorios) {

            printf("\nIndices invalidos!\n");
        }

        // Verifica se o jogador tentou atacar o próprio território.
        else if (atacanteIndex == defensorIndex) {

            printf("\nUm territorio nao pode atacar a si mesmo!\n");
        }

        // Valida para que o jogador não ataque um território da própria cor.
        else if (strcmp(mapa[atacanteIndex].cor,
                         mapa[defensorIndex].cor) == 0) {

            printf("\nNao e permitido atacar territorios da mesma cor!\n");
        }

        // Verifica se o território possui tropas suficientes para atacar.
        else if (mapa[atacanteIndex].tropas <= 1) {

            printf("\nO territorio atacante nao possui tropas suficientes!\n");
        }

        // Executa o ataque.
        else {

            atacar(&mapa[atacanteIndex],
                    &mapa[defensorIndex]);

            
            exibirMapa(mapa, quantidadeTerritorios);

        }

        // Menu de continuação do jogo.
        printf("\n========================================\n");
        printf("Deseja continuar?\n");
        printf("1 - Continuar atacando\n");
        printf("0 - Sair do jogo\n");
        printf("========================================\n");

        printf("Opcao: ");
        scanf("%d", &opcao);
        limparBuffer();

    } while (opcao != 0);

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.
    liberarMemoria(mapa);

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.
territorio* alocarMapa(int quantidade) {

    return (territorio*) calloc(quantidade, sizeof(territorio));
}

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).
void cadastrarTerritorios(territorio *mapa, int quantidade) {

    // Laço para cadastrar os territórios.
    for (int i = 0; i < quantidade; i++) {

        printf("\n=== Cadastro do Territorio %d ===\n", i + 1);

        // Entrada do nome do território.
        printf("Digite o nome do territorio: ");

        fgets(mapa[i].nome,
              sizeof(mapa[i].nome),
              stdin);

        // Remove o ENTER armazenado pelo fgets.
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Entrada da cor do exército.
        printf("Digite a cor do exercito: ");

        fgets(mapa[i].cor,
              sizeof(mapa[i].cor),
              stdin);

        // Remove o ENTER armazenado pelo fgets.
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        // Entrada da quantidade de tropas.
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);

        // Limpa o buffer do teclado.
        limparBuffer();
    }
}

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.
void liberarMemoria(territorio *mapa) {

    free(mapa);

    printf("\nMemoria liberada com sucesso!\n");
}

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.
void exibirMapa(const territorio *mapa, int quantidade) {

    printf("\n========================================\n");
    printf("            MAPA DOS TERRITORIOS\n");
    printf("========================================\n");

    for (int i = 0; i < quantidade; i++) {

        printf("[%d] %s | Exercito: %s | Tropas: %d\n",
            i + 1,
            mapa[i].nome,
            mapa[i].cor,
            mapa[i].tropas);
    }
}

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.
void atacar(territorio *atacante, territorio *defensor) {

    int dadoAtacante;
    int dadoDefensor;

    // Utiliza rand() para simular um dado de ataque (1 a 6) para cada lado.
    dadoAtacante = rand() % 6 + 1;
    dadoDefensor = rand() % 6 + 1;

    printf("\n========================================\n");
    printf("                BATALHA\n");
    printf("========================================\n");

    printf("\nTerritorio atacante: %s\n", atacante->nome);
    printf("Territorio defensor: %s\n", defensor->nome);

    printf("\nDado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    // Verifica se o atacante venceu.
    if (dadoAtacante > dadoDefensor) {

        printf("\nO atacante venceu a batalha!\n");

        // O defensor perde uma tropa.
        defensor->tropas--;

        printf("O defensor perdeu 1 tropa!\n");

        // Verifica se o defensor perdeu todas as tropas.
      if (defensor->tropas <= 0) {

            printf("\nTerritorio conquistado!\n");

            // O território defensor muda de dono.
            strcpy(defensor->cor, atacante->cor);

            // Transfere metade das tropas
            int tropasTransferidas = atacante->tropas / 2;

            defensor->tropas = tropasTransferidas;
            atacante->tropas -= tropasTransferidas;

            printf("O territorio agora pertence ao exercito %s!\n",
                defensor->cor);

            printf("Foram transferidas %d tropas para o territorio conquistado!\n",
                tropasTransferidas);
        }
    }

    // Caso o defensor vença.
    else {

        printf("\nO defensor venceu a batalha!\n");

        // O atacante perde uma tropa.
        atacante->tropas--;

        printf("O atacante perdeu 1 tropa!\n");
    }
}

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.