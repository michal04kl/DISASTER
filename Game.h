#pragma once
#include "SDL.h"
#include "button.h"
#include "SDL_ttf.h"

class Game {
private:
    SDL_Renderer* renderer;
    TTF_Font* font;

    bool isRunning;
    int** bombMap;
    int bx;
    int by;
    
    // Granice "obszaru gry"
    int startX;
    int startY;
    int endX;
    int endY;

    bool stoptime;
    bool won; // Flaga wygranej
    bool lost; //Flaga przegranej
   
    int lvl;

public:
    Game(SDL_Renderer* rend, int level);
    ~Game();
    void run(SDL_Texture* gameBackground);
    void renderTime(Uint32 startTime);

    bool hasWon();
    bool hasLost();
    void setLevelBounds(int level);
    //void render();
    void renderBombMap(int** bombMap, int x, int y);
    void buttons(SDL_Event& event, Button*** game_buttons, SDL_Texture* path, SDL_Texture* h_path);
    void destroyer(Button*** game_buttons,int y, int x);

    
};
