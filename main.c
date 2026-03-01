#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h> 

#define MAXTAILSIZE 100

#define COLOR_WALL "\033[38:5:94m"
#define COLOR_SNAKE "\033[38:5:255m"
#define COLOR_GRASS "\033[38:5:34;48:5:34m"
#define COLOR_FRUIT "\033[38:5:160m"
#define COLOR_RESET "\033[0m"


int width = 20, height = 20;
int x, y, fruitX, fruitY, score, highscore, old_highscore;
int tailX[MAXTAILSIZE], tailY[MAXTAILSIZE], nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;
bool gameover;

void Setup() {
    printf("\033[?25l");
    old_highscore = highscore;
    system("cls");
    dir = STOP;
    x = width / 2;
    y = height / 2;
    nTail = 0;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    for(int i = 0; i < MAXTAILSIZE; i++){
        tailX[i] = -1;
        tailY[i] = -1;
        
    }

}


void Draw(){
    char board[100000];
    char *p = board;
    char temp[256];

    p += sprintf(p, "\033[H");

    p += sprintf(p, "%s", COLOR_WALL);
    for (int i = 0; i < width + 2; i++) *p++ = '#';
    p += sprintf(p, "%s\n", COLOR_RESET);

    for (int i = 0; i < height; i++) {

        p += sprintf(p, "%s#%s", COLOR_WALL, COLOR_RESET); //Left wall
        
        p += sprintf(p, "%s", COLOR_GRASS);

        for (int j = 0; j < width; j++) { //Middle
            if (i == y && j == x) {
                p += sprintf(p, "%sO%s", COLOR_SNAKE, COLOR_GRASS);
            } else if (i == fruitY && j == fruitX) {

                p += sprintf(p, "%sF%s", COLOR_FRUIT, COLOR_GRASS);
            } else {
                int isTail = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == j && tailY[k] == i) {
                        p += sprintf(p, "%so%s", COLOR_SNAKE, COLOR_GRASS);
                        isTail = 1;
                        break; 
                    }
                }

                if (!isTail) {
                    *p++ = ' '; 
                }
            }
        }

        p += sprintf(p, "%s%s#%s\n", COLOR_RESET, COLOR_WALL, COLOR_RESET); // End wall
    }

    p += sprintf(p,"%s", COLOR_WALL);
    for (int i = 0; i < width + 2; i++) *p++ = '#';
    p += sprintf(p,"%s\n", COLOR_RESET);

    sprintf(temp, "Score: %d \nHighscore: %d \n",score, highscore);
    p += sprintf(p,"%s", temp);
    *p = '\0';
    printf("%s", board);
}

void Input() {
    if (_kbhit()) {
        switch (_getch()) {
            case 'a': if(nTail == 0 || dir != RIGHT)dir = LEFT; break;
            case 'd': if(nTail == 0 || dir != LEFT)dir = RIGHT; break;
            case 'w': if(nTail == 0 || dir != DOWN)dir = UP; break;
            case 's': if(nTail == 0 || dir != UP)dir = DOWN; break;
            case 'x': gameover = true; break;
        }
    }
}

void Logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;
    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
        case LEFT: x--; break;
        case RIGHT: x++; break;
        case UP: y--; break;
        case DOWN: y++; break;
        default: break;
    }

    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;
    
    //Tail detection
    for(int i = 1; i < nTail; i++){
        // If our x and y positions intersect our tail
        if(tailX[i] == x && tailY[i] == y){
            gameover = true;
        }
    }

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        if(nTail < MAXTAILSIZE) nTail++;
    }
}

void game_run(){
    Setup();
    while (!gameover) {
        Draw();
        Input();
        Logic();
        Sleep(10); // Slow down the game
    }
}
void game_over_screen(){
    highscore = score;

    system("cls");
    printf("Sorry you have lost the game!\n");
    printf("Your final score was %d", score);
    if( highscore > old_highscore){
        printf("\nYou've made a new highscore! Your highscore is %d", highscore);
    }
    _getch();
    gameover = false;
}


int main() {
    while(!gameover){
        game_run();
        game_over_screen();
    }
    return 0;
}