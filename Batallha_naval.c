#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <windows.h>

#define TAMANHO_TABULEIRO 10
#define TAMANHO_HABILIDADE 7
#define MAX_NAVIOS 5
#define MAX_HABILIDADES 3

// Códigos de cores ANSI melhorados
#define RESET   "\033[0m"
#define BRIGHT  "\033[1m"
#define DIM     "\033[2m"

// Cores de fundo
#define BG_BLUE     "\033[48;5;27m"
#define BG_GREEN    "\033[48;5;34m"
#define BG_RED      "\033[48;5;196m"
#define BG_YELLOW   "\033[48;5;226m"
#define BG_PURPLE   "\033[48;5;129m"
#define BG_ORANGE   "\033[48;5;208m"
#define BG_DARKBLUE "\033[48;5;18m"

// Cores de texto
#define TXT_WHITE   "\033[38;5;255m"
#define TXT_BLUE    "\033[38;5;33m"
#define TXT_GOLD    "\033[38;5;220m"

typedef struct {
    int linha;
    int coluna;
    int tamanho;
    char direcao;
    bool destruido;
} Navio;

typedef struct {
    int tipo;
    int linha;
    int coluna;
} Habilidade;

int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
Navio navios[MAX_NAVIOS];
Habilidade habilidades[MAX_HABILIDADES];
int numNavios = 0;
int numHabilidades = 0;

// Protótipos de funções
void inicializarTabuleiro();
void exibirTabuleiro(bool mostrarNavios);
bool posicionarNavio(int linha, int coluna, int tamanho, char direcao);
bool aplicarHabilidade(int tipo, int linha, int coluna);
void criarHabilidadeCone(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void criarHabilidadeCruz(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void criarHabilidadeOctaedro(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]);
void atirar(int linha, int coluna);
void menuPrincipal();
void menuPosicionarNavios();
void menuAplicarHabilidades();
void limparBuffer();
void limparTela();
void mostrarCabecalho();
void mostrarLegenda();
void desenharBarco();
void animacaoImpacto();
void animacaoHabilidade(int tipo);

int main() {
    // Configuração inicial para suporte a cores no Windows
    system("cls");
    SetConsoleOutputCP(65001);
    
    inicializarTabuleiro();
    menuPrincipal();
    return 0;
}

void inicializarTabuleiro() {
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = 0;
        }
    }
    numNavios = 0;
    numHabilidades = 0;
}

void exibirTabuleiro(bool mostrarNavios) {
    printf("\n   ");
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf(BRIGHT TXT_GOLD "%2d " RESET, i);
    }
    printf("\n");
    
    for (int i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf(BRIGHT TXT_GOLD "%2d " RESET, i);
        for (int j = 0; j < TAMANHO_TABULEIRO; j++) {
            switch (tabuleiro[i][j]) {
                case 0: // Água
                    printf(BG_BLUE TXT_WHITE " ~ " RESET);
                    break;
                case 1: // Acerto
                    printf(BG_RED TXT_WHITE BRIGHT " X " RESET);
                    break;
                case 2: // Tiro na água
                    printf(BG_DARKBLUE TXT_WHITE " O " RESET);
                    break;
                case 3: // Navio (se mostrarNavios for true)
                    if (mostrarNavios) {
                        printf(BG_GREEN TXT_WHITE BRIGHT " S " RESET);
                    } else {
                        printf(BG_BLUE TXT_WHITE " ~ " RESET);
                    }
                    break;
                case 5: // Área de habilidade
                    printf(BG_PURPLE TXT_WHITE " * " RESET);
                    break;
                default:
                    printf(BG_BLUE "   " RESET);
            }
        }
        printf("\n");
    }
}

bool posicionarNavio(int linha, int coluna, int tamanho, char direcao) {
    if (numNavios >= MAX_NAVIOS) return false;

    // Verificação de limites
    switch (toupper(direcao)) {
        case 'H': if (coluna + tamanho > TAMANHO_TABULEIRO) return false; break;
        case 'V': if (linha + tamanho > TAMANHO_TABULEIRO) return false; break;
        case 'D': if (linha + tamanho > TAMANHO_TABULEIRO || coluna + tamanho > TAMANHO_TABULEIRO) return false; break;
        case 'E': if (linha + tamanho > TAMANHO_TABULEIRO || coluna - tamanho + 1 < 0) return false; break;
        default: return false;
    }

    // Verificação de sobreposição
    for (int i = 0; i < tamanho; i++) {
        int x = linha, y = coluna;
        
        switch (toupper(direcao)) {
            case 'H': y += i; break;
            case 'V': x += i; break;
            case 'D': x += i; y += i; break;
            case 'E': x += i; y -= i; break;
        }
        
        if (tabuleiro[x][y] != 0) return false;
    }

    // Posicionamento
    for (int i = 0; i < tamanho; i++) {
        int x = linha, y = coluna;
        
        switch (toupper(direcao)) {
            case 'H': y += i; break;
            case 'V': x += i; break;
            case 'D': x += i; y += i; break;
            case 'E': x += i; y -= i; break;
        }
        
        tabuleiro[x][y] = 3;
    }

    // Registra o navio
    navios[numNavios].linha = linha;
    navios[numNavios].coluna = coluna;
    navios[numNavios].tamanho = tamanho;
    navios[numNavios].direcao = toupper(direcao);
    navios[numNavios].destruido = false;
    numNavios++;
    
    return true;
}

bool aplicarHabilidade(int tipo, int linha, int coluna) {
    if (numHabilidades >= MAX_HABILIDADES) return false;

    int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    int offset = TAMANHO_HABILIDADE / 2;

    // Cria a matriz de habilidade
    switch (tipo) {
        case 1: criarHabilidadeCone(habilidade); break;
        case 2: criarHabilidadeCruz(habilidade); break;
        case 3: criarHabilidadeOctaedro(habilidade); break;
        default: return false;
    }

    // Verifica limites
    if (linha - offset < 0 || linha + offset >= TAMANHO_TABULEIRO ||
        coluna - offset < 0 || coluna + offset >= TAMANHO_TABULEIRO) {
        return false;
    }

    // Aplica habilidade
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            if (habilidade[i][j] == 1) {
                int x = linha - offset + i;
                int y = coluna - offset + j;
                if (tabuleiro[x][y] == 0) {
                    tabuleiro[x][y] = 5;
                }
            }
        }
    }

    // Registra a habilidade
    habilidades[numHabilidades].tipo = tipo;
    habilidades[numHabilidades].linha = linha;
    habilidades[numHabilidades].coluna = coluna;
    numHabilidades++;
    
    animacaoHabilidade(tipo);
    return true;
}

void criarHabilidadeCone(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            habilidade[i][j] = (j >= (TAMANHO_HABILIDADE/2 - i/2) && j <= (TAMANHO_HABILIDADE/2 + i/2)) ? 1 : 0;
        }
    }
}

void criarHabilidadeCruz(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            habilidade[i][j] = (i == centro || j == centro) ? 1 : 0;
        }
    }
}

void criarHabilidadeOctaedro(int habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int centro = TAMANHO_HABILIDADE / 2;
    for (int i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (int j = 0; j < TAMANHO_HABILIDADE; j++) {
            habilidade[i][j] = (abs(i - centro) + abs(j - centro) <= centro) ? 1 : 0;
        }
    }
}

void atirar(int linha, int coluna) {
    if (linha < 0 || coluna < 0 || linha >= TAMANHO_TABULEIRO || coluna >= TAMANHO_TABULEIRO) {
        printf("Posição inválida!\n");
        return;
    }

    if (tabuleiro[linha][coluna] == 1 || tabuleiro[linha][coluna] == 2) {
        printf("Você já atirou aqui!\n");
        return;
    }

    if (tabuleiro[linha][coluna] == 3) {
        tabuleiro[linha][coluna] = 1;
        animacaoImpacto();
        printf(BRIGHT BG_RED TXT_WHITE " ACERTOU! " RESET "\n");
        
        // Verifica se navio foi destruído
        for (int i = 0; i < numNavios; i++) {
            bool destruido = true;
            for (int j = 0; j < navios[i].tamanho; j++) {
                int x = navios[i].linha, y = navios[i].coluna;
                
                switch (navios[i].direcao) {
                    case 'H': y += j; break;
                    case 'V': x += j; break;
                    case 'D': x += j; y += j; break;
                    case 'E': x += j; y -= j; break;
                }
                
                if (tabuleiro[x][y] != 1) {
                    destruido = false;
                    break;
                }
            }
            
            if (destruido && !navios[i].destruido) {
                navios[i].destruido = true;
                printf(BRIGHT BG_RED TXT_WHITE " NAVIO DESTRUÍDO! " RESET "\n");
                break;
            }
        }
    } else {
        tabuleiro[linha][coluna] = 2;
        printf(BG_DARKBLUE TXT_WHITE " ÁGUA! " RESET "\n");
    }
}

void menuPrincipal() {
    char opcao;
    do {
        limparTela();
        mostrarCabecalho();
        
        printf(BRIGHT TXT_BLUE "\n  === MENU PRINCIPAL ===\n" RESET);
        printf("\n  1. Posicionar Navios");
        printf("\n  2. Aplicar Habilidades");
        printf("\n  3. Atirar");
        printf("\n  4. Visualizar Tabuleiro");
        printf("\n  5. Reiniciar Jogo");
        printf("\n  6. Sair");
        
        printf(BRIGHT TXT_GOLD "\n\n Escolha uma opção: " RESET);
        opcao = getch();
        printf("%c\n", opcao);
        
        switch (opcao) {
            case '1': menuPosicionarNavios(); break;
            case '2': menuAplicarHabilidades(); break;
            case '3': {
                int linha, coluna;
                limparTela();
                desenharBarco();
                exibirTabuleiro(false);
                printf(BRIGHT "\n COORDENADAS DO ATAQUE\n" RESET);
                printf(" Linha (0-9): ");
                scanf("%d", &linha);
                printf(" Coluna (0-9): ");
                scanf("%d", &coluna);
                limparBuffer();
                atirar(linha, coluna);
                printf("\n Pressione qualquer tecla...");
                getch();
                break;
            }
            case '4':
                limparTela();
                desenharBarco();
                exibirTabuleiro(true);
                mostrarLegenda();
                printf("\n Pressione qualquer tecla...");
                getch();
                break;
            case '5':
                inicializarTabuleiro();
                printf(BRIGHT TXT_GOLD "\n Jogo reiniciado!" RESET);
                Sleep(1500);
                break;
            case '6':
                printf(BRIGHT "\n Saindo do jogo..." RESET);
                Sleep(1000);
                break;
            default:
                printf(BRIGHT BG_RED "\n Opção inválida!" RESET);
                Sleep(1000);
                break;
        }
    } while (opcao != '6');
}

void menuPosicionarNavios() {
    char opcao;
    do {
        limparTela();
        desenharBarco();
        printf(BRIGHT TXT_BLUE "\n  === POSICIONAR NAVIOS ===\n" RESET);
        printf("  Navios colocados: %d/%d\n", numNavios, MAX_NAVIOS);
        
        printf("\n  1. Horizontal");
        printf("\n  2. Vertical");
        printf("\n  3. Diagonal Direita");
        printf("\n  4. Diagonal Esquerda");
        printf("\n  5. Voltar");
        
        printf(BRIGHT TXT_GOLD "\n\n Escolha: " RESET);
        opcao = getch();
        printf("%c\n", opcao);
        
        if (opcao >= '1' && opcao <= '4') {
            if (numNavios >= MAX_NAVIOS) {
                printf(BRIGHT BG_RED "\n Limite de navios atingido!" RESET);
                Sleep(1500);
                continue;
            }
            
            int linha, coluna, tamanho;
            char direcao;
            
            limparTela();
            desenharBarco();
            exibirTabuleiro(true);
            
            printf(BRIGHT "\n POSICIONAR NAVIO %s\n" RESET,
                (opcao == '1') ? "HORIZONTAL" :
                (opcao == '2') ? "VERTICAL" :
                (opcao == '3') ? "DIAGONAL DIREITA" : "DIAGONAL ESQUERDA");
            
            printf("\n Linha (0-9): ");
            scanf("%d", &linha);
            printf(" Coluna (0-9): ");
            scanf("%d", &coluna);
            printf(" Tamanho: ");
            scanf("%d", &tamanho);
            limparBuffer();
            
            direcao = (opcao == '1') ? 'H' :
                     (opcao == '2') ? 'V' :
                     (opcao == '3') ? 'D' : 'E';
            
            if (posicionarNavio(linha, coluna, tamanho, direcao)) {
                printf(BRIGHT BG_GREEN TXT_WHITE "\n Navio posicionado!" RESET);
            } else {
                printf(BRIGHT BG_RED TXT_WHITE "\n Posição inválida!" RESET);
            }
            Sleep(1500);
        }
    } while (opcao != '5');
}

void menuAplicarHabilidades() {
    char opcao;
    do {
        limparTela();
        desenharBarco();
        printf(BRIGHT TXT_BLUE "\n  === HABILIDADES ESPECIAIS ===\n" RESET);
        printf("  Habilidades usadas: %d/%d\n", numHabilidades, MAX_HABILIDADES);
        
        printf("\n  1. Cone de Ataque");
        printf("\n  2. Cruz de Bombardeio");
        printf("\n  3. Losango Tático");
        printf("\n  4. Voltar");
        
        printf(BRIGHT TXT_GOLD "\n\n Escolha: " RESET);
        opcao = getch();
        printf("%c\n", opcao);
        
        if (opcao >= '1' && opcao <= '3') {
            if (numHabilidades >= MAX_HABILIDADES) {
                printf(BRIGHT BG_RED "\n Limite de habilidades atingido!" RESET);
                Sleep(1500);
                continue;
            }
            
            int linha, coluna;
            
            limparTela();
            desenharBarco();
            exibirTabuleiro(true);
            
            printf(BRIGHT "\n APLICAR %s\n" RESET,
                (opcao == '1') ? "CONE DE ATAQUE" :
                (opcao == '2') ? "CRUZ DE BOMBARDEIO" : "LOSANGO TÁTICO");
            
            printf("\n Linha central (0-9): ");
            scanf("%d", &linha);
            printf(" Coluna central (0-9): ");
            scanf("%d", &coluna);
            limparBuffer();
            
            if (aplicarHabilidade(opcao - '0', linha, coluna)) {
                printf(BRIGHT BG_PURPLE TXT_WHITE "\n Habilidade aplicada!" RESET);
            } else {
                printf(BRIGHT BG_RED TXT_WHITE "\n Posição inválida!" RESET);
            }
            Sleep(1500);
        }
    } while (opcao != '4');
}

void mostrarCabecalho() {
    printf(BRIGHT BG_DARKBLUE TXT_GOLD);
    printf("  ╔══════════════════════════════════════╗\n");
    printf("  ║                                      ║\n");
    printf("  ║             BATALHA NAVAL            ║\n");
    printf("  ║                                      ║\n");
    printf("  ╚══════════════════════════════════════╝\n");
    printf(RESET);
}

void mostrarLegenda() {
    printf(BRIGHT "\n  LEGENDA:\n" RESET);
    printf("  " BG_BLUE TXT_WHITE " ~ " RESET " Água\n");
    printf("  " BG_GREEN TXT_WHITE " S " RESET " Seu Navio\n");
    printf("  " BG_RED TXT_WHITE " X " RESET " Acerto\n");
    printf("  " BG_DARKBLUE TXT_WHITE " O " RESET " Tiro na Água\n");
    printf("  " BG_PURPLE TXT_WHITE " * " RESET " Área de Habilidade\n");
}

void desenharBarco() {
    printf(BRIGHT TXT_BLUE);
    printf("      . . . . o o o o\n");
    printf("            _____      o       _____\n");
    printf("          _/____ \\____         /____ \\_____\n");
    printf("          \\  ''  /    '\\       ''  /    '' \\\n");
    printf("    ~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^~^\n");
    printf(RESET);
}

void animacaoImpacto() {
    for (int i = 0; i < 3; i++) {
        limparTela();
        printf(BRIGHT BG_RED TXT_WHITE "\n\n      BOOOOOM!      \n\n" RESET);
        Sleep(200);
        limparTela();
        printf(BRIGHT BG_YELLOW TXT_WHITE "\n\n      BOOOOOM!      \n\n" RESET);
        Sleep(200);
    }
}

void animacaoHabilidade(int tipo) {
    const char* formas[3] = {
        "  /\\\n /  \\\n/    \\\n\\    /\n \\  /\n  \\/",
        "  |\n--+--\n  |",
        "  /\\\n /  \\\n \\  /\n  \\/"
    };
    
    for (int i = 0; i < 2; i++) {
        limparTela();
        printf(BRIGHT BG_PURPLE TXT_WHITE "\n\n");
        printf("%s", formas[tipo-1]);
        printf("\n\n HABILIDADE ATIVADA! \n\n" RESET);
        Sleep(300);
        limparTela();
        printf(BRIGHT BG_ORANGE TXT_WHITE "\n\n");
        printf("%s", formas[tipo-1]);
        printf("\n\n HABILIDADE ATIVADA! \n\n" RESET);
        Sleep(300);
    }
}

void limparBuffer() {
    while (getchar() != '\n');
}

void limparTela() {
    system("cls || clear");
}