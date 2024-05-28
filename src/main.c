#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "keyboard.h"
#include "screen.h"
#include "timer.h"

#define MAXX 80
#define MAXY 24
#define MINX 0
#define MINY 0

#define HORSE_WIDTH 14

char cavalo[6][HORSE_WIDTH + 1] = {
    {"        ,--,\n"},
    {"  _ ___/ /|\n"},
    {" ;( )__, )\n"},
    {"; //   | '--;\n"},
    {"  /    |\n"},
    {"  ^    ^\n"},
};

char ground[MAXX + 1];
char obstacle = '#';

// Printa da matriz do cavalo
void printHorse(int x, int y) {
    for (int i = 0; i < 6; i++) {
        screenGotoxy(x, y + i);
        printf("%s", cavalo[i]);
    }
}

// Limpa tela do cavalo
void clearHorse(int x, int y) {
    for (int i = 0; i < 6; i++) {
        screenGotoxy(x, y + i);
        printf("              \n");
    }
}

// criar a matriz chão
void initGround() {
    for (int i = 0; i < MAXX; i++) {
        ground[i] = '_';
    }
    ground[MAXX] = '\0';
}

// Printa o chão
void drawGround() {
    screenGotoxy(0, MAXY - 2);
    printf("%s\n", ground);
}

// Desenha os obstaculos
void drawObstacles(int *obstacles, int numObstacles) {
    for (int i = 0; i < numObstacles; i++) {
        if (obstacles[i] >= MINX && obstacles[i] < MAXX) {
            screenGotoxy(obstacles[i], MAXY - 3);
            printf("%c", obstacle);
        }
    }
}

// Limpa o frame do obstaculos
void clearObstacles(int *obstacles, int numObstacles) {
    for (int i = 0; i < numObstacles; i++) {
        if (obstacles[i] >= MINX && obstacles[i] < MAXX) {
            screenGotoxy(obstacles[i], MAXY - 3);
            printf(" ");
        }
    }
}

// Checagem da colisão, desafio ajustar o Range do cavalo, verificar a altura e largura do Cavalo por x e y
int checkCollision(int horseX, int horseY, int *obstacles, int numObstacles) {
    for (int i = 0; i < numObstacles; i++) {
        // Verifica largura do cavalo
        if (obstacles[i] >= horseX && obstacles[i] < horseX + HORSE_WIDTH) {
            // Verifica altura do cavalo
            if (horseY >= MAXY - 8) {
                return 1;
            }
        }
    }
    return 0;
}

// Timer no canto da tela
void printTimer(int elapsedTime) {
    int minutes = elapsedTime / 60;
    int seconds = elapsedTime % 60;
    screenGotoxy(MAXX - 10, 0); // Posição do Timer
    printf("Time: %02d:%02d", minutes, seconds);
}

// Printa "Game Over" e o tempo que o jogador ficou vivo
void showGameOver(int elapsedTime) {
    screenClear();
    screenGotoxy(MAXX / 2 - 5, MAXY / 2);
    printf("Game Over!\n");
    
    screenGotoxy(MAXX / 2 - 10, MAXY / 2 + 1);
    printf("Você sobreviveu por %d segundos.\n", elapsedTime);

    screenUpdate();
}

int main() {
    static int ch = 0;
    int x = 5;          // Ajuste da posição inicial pra não ficar mt perto da parede
    int y = MAXY - 8;   // Seta a posição inicial em y
    int jumping = 0;    // Boolean para "está_Pulando?"
    int jumpCounter = 0;// Contador para Gravidade

    screenInit(1);
    keyboardInit();
    timerInit(50);
    initGround();

    int numObstacles = 1;        // Qtd de Obstaculos
    
    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Gera os dois números aleatórios entre 0 e 80
    int num1 = rand() % 80;

    if (num1 < 30){ // garante que ele seja maior q 30
        num1 += 30;
    }

    int obstacles[1] = {num1}; // Posição dos Obstaculos

    time_t startTime = time(NULL); // Inicializa o timer

    printHorse(x, y);   // Printa o cavalo na posição inicial
    drawGround();       // Deseja o chão
    drawObstacles(obstacles, numObstacles); // Inicializa os Obstaculos
    screenUpdate();

    while (ch != 10) { // Chave de Quit é o ENTER
        // Leitor de entrada do Usuário
        if (keyhit()) {
            ch = readch();
            if (ch == 'w' && !jumping) {
                jumping = 1; // Jumping == True
                jumpCounter = 12; // Tamanho do Pulo
            }
        }
        // Atualizar estado do jogo (mover elementos, checkar colisão, etc...)
        if (timerTimeOver() == 1) {
            clearHorse(x, y);
            clearObstacles(obstacles, numObstacles);

            // Pulo e Gravidade
            if (jumping) {
                if (jumpCounter > 0) {
                    y -= 1; // Increase the jump height
                    jumpCounter--;
                } else {
                    y += 1; // Increase the falling speed
                    if (y >= MAXY - 8) {
                        jumping = 0;
                        y = MAXY - 8; // Reset to above ground level
                    }
                }
            }

            // Movimentação dos Obstaculos
            for (int i = 0; i < numObstacles; i++) {
                obstacles[i] -= 1;
                if (obstacles[i] < MINX) {
                    obstacles[i] = MAXX; // Respawnar o obstaculo
                }
            }

            // Check da Colisão
            if (checkCollision(x, y, obstacles, numObstacles)) {
                int elapsedTime = (int)(time(NULL) - startTime);
                showGameOver(elapsedTime);
                break;
            }

            printHorse(x, y);
            drawGround();
            drawObstacles(obstacles, numObstacles);
            
            // Calcula e mostra o tempo que o jogador perdeu
            int elapsedTime = (int)(time(NULL) - startTime);
            printTimer(elapsedTime);

            screenUpdate();
        }
    }

    keyboardDestroy();
    //screenDestroy();
    timerDestroy();
    return 0;
}
