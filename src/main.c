#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>


#define WINDOW_WIDTH 700
#define WINDOW_HEIGHT 400
#define GRAVITY 200.0f

SDL_Window* window;
SDL_Renderer* renderer;

struct firework{
    float x;
    float y;
    float speedx;
    float speedy;
    int size;
    int R;
    int G;
    int B;
}; typedef struct firework Firework;

Firework firework[70][100];

int fireworkArraySize = 50;
int fireworkSize = 100;

float RandomFloat(float max){
    return (float) ((rand() / (float) RAND_MAX)) * max;
}

void createFirework(){
    fireworkArraySize = (rand() % (70 - 50 + 1)) + 50;
    for (int i = 0; i < fireworkArraySize; i++){
        float randomX = RandomFloat(WINDOW_WIDTH);
        float randomY = RandomFloat(WINDOW_HEIGHT);
        int r = (int) RandomFloat(255);
        int g = (int) RandomFloat(255);
        int b = (int) RandomFloat(255);
        for (int j = 0; j < fireworkSize; j++){
            firework[i][j].x = randomX;
            firework[i][j].y = randomY;
            float angle = RandomFloat(3.1415f * 2.0f);
            float power = RandomFloat(100.0f);
            firework[i][j].speedx = cosf(angle) * power;
            firework[i][j].speedy = sinf(angle) * power;
            firework[i][j].size = (int) RandomFloat(5);
            firework[i][j].R = r;
            firework[i][j].G = g;
            firework[i][j].B = b;
        }
    }
}

void updateFirework(float elapsed){
    for (int i = 0; i < fireworkArraySize; i++){
        for (int j = 0; j < fireworkSize; j++){
            firework[i][j].x += firework[i][j].speedx * elapsed;
            firework[i][j].y += firework[i][j].speedy * elapsed;
            firework[i][j].speedy += GRAVITY * elapsed;
            if (firework[i][j].speedy >= 200.0f){
                    firework[i][j].R--;
                    firework[i][j].G--;
                    firework[i][j].B--;
                    firework[i][j].R = firework[i][j].R >= 0 ? firework[i][j].R : 0;
                    firework[i][j].G = firework[i][j].G >= 0 ? firework[i][j].G : 0;
                    firework[i][j].B = firework[i][j].B >= 0 ? firework[i][j].B : 0;
            }

            if (firework[i][j].y >= WINDOW_HEIGHT + 500){
                createFirework();
            }
        }
    }
}

void renderFirework(){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    
    for (int i = 0; i < fireworkArraySize; i++){
        for (int j = 0; j < fireworkSize; j++){
            SDL_SetRenderDrawColor(renderer, firework[i][j].R, firework[i][j].G, firework[i][j].B, 0);
            SDL_Rect rect = {firework[i][j].x, firework[i][j].y, 
                            firework[i][j].size, firework[i][j].size};
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    
    
    SDL_RenderPresent(renderer);
}

void update(float elapsed){
    updateFirework(elapsed);
    renderFirework();
}

void createWindow(){
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("Firework", 
    SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
    WINDOW_WIDTH, WINDOW_HEIGHT, 
    SDL_WINDOW_OPENGL);
    
    if (window == NULL){
        printf("Error create window.\n");
        exit(1);

    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL){
        printf("Error create renderer.\n");
        exit(1);
    }
}

void destroyAll(){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void start(){

    srand(time(NULL));

    createFirework();


    SDL_bool isRunning = SDL_TRUE;
    SDL_Event event;
    createWindow();

    Uint32 lastTicks = SDL_GetTicks();
    while(isRunning){
        while(SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                isRunning = SDL_FALSE;
            }
        }
        Uint32 currentTicks = SDL_GetTicks();
        Uint32 diff = currentTicks - lastTicks;
        float elapsed = diff / 1000.0f;
        update(elapsed);
        lastTicks = currentTicks;


    }
    destroyAll();
}

int main(int argc, char* argv[]){

    start();

    return 0;

}







