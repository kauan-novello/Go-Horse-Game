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
void printHorse(int x, int y)
{
    for (int i = 0; i < 6; i++)
    {
        screenGotoxy(x, y + i);
        printf("%s", cavalo[i]);
    }
}

// Limpa tela do cavalo
void clearHorse(int x, int y)
{
    for (int i = 0; i < 6; i++)
    {
        screenGotoxy(x, y + i);
        printf("              \n");
    }
}

// criar a matriz chão
void initGround()
{
    for (int i = 0; i < MAXX; i++)
    {
        ground[i] = '=';
    }
    ground[MAXX] = '\0';
}

// Printa o chão
void drawGround()
{
    screenGotoxy(0, MAXY - 2);
    printf("%s\n", ground);
}

// Desenha os obstaculos
void drawObstacles(int *obstacles, int numObstacles)
{
    for (int i = 0; i < numObstacles; i++)
    {
        if (obstacles[i] >= MINX && obstacles[i] < MAXX)
        {
            screenGotoxy(obstacles[i], MAXY - 3);
            printf("%c", obstacle);
        }
    }
}

// Limpa o frame do obstaculos
void clearObstacles(int *obstacles, int numObstacles)
{
    for (int i = 0; i < numObstacles; i++)
    {
        if (obstacles[i] >= MINX && obstacles[i] < MAXX)
        {
            screenGotoxy(obstacles[i], MAXY - 3);
            printf(" ");
        }
    }
}

// Checagem da colisão, desafio ajustar o Range do cavalo, verificar a altura e largura do Cavalo por x e y
int checkCollision(int horseX, int horseY, int *obstacles, int numObstacles)
{
    for (int i = 0; i < numObstacles; i++)
    {
        // Verifica largura do cavalo
        if (obstacles[i] >= horseX && obstacles[i] < horseX + HORSE_WIDTH)
        {
            // Verifica altura do cavalo
            if (horseY >= MAXY - 8)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Timer no canto da tela
void printTimer(int elapsedTime)
{
    struct tempo
    {
        int minutes;
        int seconds;
    } tempo;

    tempo.minutes = elapsedTime / 60;
    tempo.seconds = elapsedTime % 60;

    screenGotoxy(MAXX - 10, 0); // Posição do Timer
    printf("Time: %02d:%02d", tempo.minutes, tempo.seconds);
}

// Printa "Game Over" e o tempo que o jogador ficou vivo
void showGameOver(int elapsedTime)
{
    screenClear();
    printf("Game Over!\n");
    printf("Você sobreviveu por %d segundos.\n", elapsedTime);

    screenUpdate();
}
int funcaoJogo()
{
    static int ch = 0;
    int x = 5;           // Ajuste da posição inicial pra não ficar mt perto da parede
    int y = MAXY - 8;    // Seta a posição inicial em y
    int jumping = 0;     // Boolean para "está_Pulando?"
    int jumpCounter = 0; // Contador para Gravidade

    screenInit(1);
    keyboardInit();
    timerInit(50);
    initGround();

    int numObstacles = 1; // Qtd de Obstaculos

    // Inicializa a semente do gerador de números aleatórios
    srand(time(NULL));

    // Gera os dois números aleatórios entre 0 e 80
    int num1 = rand() % 80;

    if (num1 < 30)
    { // garante que ele seja maior q 30
        num1 += 30;
    }

    int obstacles[1] = {num1}; // Qtd e posição dos Obstaculos

    time_t startTime = time(NULL); // Inicializa o timer

    printHorse(x, y);                       // Printa o cavalo na posição inicial
    drawGround();                           // Deseja o chão
    drawObstacles(obstacles, numObstacles); // Inicializa os Obstaculos
    screenUpdate();

    while (ch != 10)
    { // Chave de Quit é o ENTER
        // Leitor de entrada do Usuário
        if (keyhit())
        {
            ch = readch();
            if (ch == 'w' && !jumping)
            {
                jumping = 1;      // Jumping == True
                jumpCounter = 9; // Tamanho do Pulo
            }
        }
        // Atualizar estado do jogo (mover elementos, checkar colisão, etc...)
        if (timerTimeOver() == 1)
        {
            clearHorse(x, y);
            clearObstacles(obstacles, numObstacles);

            // Pulo e Gravidade
            if (jumping)
            {
                if (jumpCounter > 0)
                {
                    y -= 1; // Altura do salto
                    jumpCounter--;
                }
                else
                {
                    y += 1; // Gravidade
                    if (y >= MAXY - 8)
                    {
                        jumping = 0;
                        y = MAXY - 8; // Volta pro chão
                    }
                }
            }

            // Movimentação dos Obstaculos
            for (int i = 0; i < numObstacles; i++)
            {
                obstacles[i] -= 1;
                if (obstacles[i] < MINX)
                {
                    obstacles[i] = MAXX; // Respawnar o obstaculo
                }
            }

            // Check da Colisão
            if (checkCollision(x, y, obstacles, numObstacles))
            {
                int elapsedTime = (int)(time(NULL) - startTime);
                system("clear");
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
    timerDestroy();
    return 0;
}
int funcaoFeedback()
{
    char feedback[1000];
    FILE *arquivo;

    arquivo = fopen("feedback.txt", "a");

    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return 0;
    }

    printf("Por favor, forneça seu feedback sobre o jogo:\n");

    // Limpar o buffer de entrada
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;

    // Use fgets para ler uma linha inteira de feedback
    fgets(feedback, sizeof(feedback), stdin);

    // Remova a quebra de linha (\n) do final do feedback, se presente
    int len = strlen(feedback);
    if (len > 0 && feedback[len - 1] == '\n')
    {
        feedback[len - 1] = '\0';
    }

    fprintf(arquivo, "%s\n", feedback); // Escreva o feedback no arquivo

    fclose(arquivo);

    printf("Feedback registrado com sucesso. Obrigado!\n");
}

int menu()
{

    char *palavra = malloc(15 * sizeof(char)); // Aloca dinamica para uma string
    if (palavra == NULL)
    {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    printf("Digite 'start' para começar o jogo \nou Digite 'feedback' para adicionar um comentário \nou Digite 'quit' para sair \n\n");
    scanf("%14s", palavra);

    if (strcmp(palavra, "start") == 0)
    {   
        printf("Use a tecla 'w' para fazer seu cavalo pular sobre os obstáculos.");
        funcaoJogo();
        strcpy(palavra, "quit");
    }
    if (strcmp(palavra, "feedback") == 0)
    {
        funcaoFeedback();
        sleep(2);
        strcpy(palavra, "quit");
    }
    if (strcmp(palavra, "quit") == 0)
    {
        return 0;
    }
    else
    {
        system("clear");
        printf("\nDesculpe não consegui entender a palavra vamos tentar novamente\n\n");
        sleep(2);
        menu();
    }
}

int main()
{
    system("clear");
    printf("Olá Bem vindo ao GO Horse\n");
    for (int i = 0; i < 6; i++)
    {
        printf("%s", cavalo[i]);
    }
    printf("\n");
    menu();
    return 0;
}
