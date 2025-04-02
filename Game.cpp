#include "Game.h"
#include <cstdlib>
#include <ctime>
#include "SDL_image.h"
#include <iostream>
#include <string>
#include "bomby.h"
#include "button.h"
#include "SDL_ttf.h"
#include <fstream>
using namespace std;

Game::Game(SDL_Renderer* rend, int level) : renderer(rend), isRunning(true) {

    setLevelBounds(level);
    //rozmiary planszy zale¿nie od levelu
    if (level == 1) {
        bx = 10;
        by = 10;
    }
    else {
        bx = 20;
        by = 10;
    }
    bombMap = bomby(level);

    //czcionki
    font = TTF_OpenFont("czcionki/FoxFont.ttf", 24);
}

//wczytanie tekstur mapy
void Game::renderBombMap(int** bombMap, int x, int y) {
    int box = 30; // rozmiar kafelka - 30x30 px
    SDL_Texture* bomb = IMG_LoadTexture(renderer, "zdj/b.jpg");
    SDL_Texture* number[9];
    for (int i = 0; i < 9; i++) {
        string path = "zdj/" + to_string(i) + ".jpg";
        number[i] = IMG_LoadTexture(renderer, path.c_str());
    }
    for (int i = 0; i < x;i++) {
        for (int j = 0; j < y; j++) {
            SDL_Rect rect = { startX + i * box,startY + j * box, box, box };
            if (bombMap[i][j] == -1) {
                SDL_RenderCopy(renderer, bomb, NULL, &rect);
            } else {
                SDL_RenderCopy(renderer, number[bombMap[i][j]], NULL, &rect);
            }
        }
    }
    SDL_DestroyTexture(bomb);
    for (int i = 0; i < 9; i++) {
        SDL_DestroyTexture(number[i]);
    }
}

Game::~Game() {}


void Game::setLevelBounds(int level){
    lvl = level;
    switch (level) {
    case 1:
        startX = 234; // Pocz¹tkowa pozycja X
        startY = 144; // Pocz¹tkowa pozycja Y
        endX = startX + 282; // Koñcowa pozycja X
        endY = startY + 282; // Koñcowa pozycja Y
        break;
    default:
        startX = 93; // Pocz¹tkowa pozycja X
        startY = 144; // Pocz¹tkowa pozycja Y
        endX = startX + 564; // Koñcowa pozycja X
        endY = startY + 282; // Koñcowa pozycja Y
        break;
    }

}




void Game::buttons(SDL_Event& event, Button*** game_buttons, SDL_Texture* path, SDL_Texture* h_path) {
    //obs³uga przycisków gry
    for (int i = 0; i < bx; i++) {
        for (int j = 0; j < by; j++) {
            game_buttons[i][j]->handleEvent(event);
            if (game_buttons[i][j]->isClicked) {
                if (bombMap[i][j] == -1) {
                    game_buttons[i][j]->~Button();
                    game_buttons[i][j]->wasClicked = true;
                    game_buttons[i][j]->isClicked = false;
                    //przegrana
                    lost = true;
                    isRunning = false;
                }else if(!game_buttons[i][j]->wasClicked) {
                    game_buttons[i][j]->~Button();
                    game_buttons[i][j]->wasClicked = true;
                    game_buttons[i][j]->isClicked = false;
                    //can be path - self
                    if (i == 0) { game_buttons[i][j]->canPath = true; }
                }
            }
            if (game_buttons[i][j]->isRightClicked && game_buttons[i][j]->wasClicked && !game_buttons[i][j]->isPath && game_buttons[i][j]->canPath) {
                if (bombMap[i][j] == -1) {
                    //przegrana
                    
                    lost = true;
                    isRunning = false;
                    
                } else if(i == bx-1){
                    //wygrana
                    stoptime = true;
                    game_buttons[i][j]->changeTexture(path, h_path);
                    game_buttons[i][j]->isRightClicked = false;
                    game_buttons[i][j]->isPath = true;
                    won = true;
                    isRunning = false;
                } else {
                    game_buttons[i][j]->changeTexture(path,h_path);
                    game_buttons[i][j]->isRightClicked = false;
                    game_buttons[i][j]->isPath = true;
                    //can be path - dooko³a
                    if (j - 1 >= 0) { game_buttons[i][j - 1]->canPath = true; }
                    if (j - 1 >= 0 && i - 1 >= 0) { game_buttons[i-1][j - 1]->canPath = true; }
                    if (j - 1 >= 0 && i+1 < bx) { game_buttons[i+1][j - 1]->canPath = true; }

                    if (i - 1 >= 0) { game_buttons[i - 1][j]->canPath = true; }
                    if (i + 1 < bx) { game_buttons[i+1][j]->canPath = true; }

                    if (j + 1 < by) { game_buttons[i][j + 1]->canPath = true; }
                    if (j + 1 < by && i - 1 >= 0 ) { game_buttons[i-1][j + 1]->canPath = true; }
                    if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->canPath = true; }
                }
            } else if (game_buttons[i][j]->isPath && game_buttons[i][j]->isRightClicked) {
                game_buttons[i][j]->buttonTexture=IMG_LoadTexture(renderer, "");
                game_buttons[i][j]->buttonHoverTexture = IMG_LoadTexture(renderer, "");
                game_buttons[i][j]->isPath = false;
                game_buttons[i][j]->isRightClicked = false;
                //destroy path, efekt domina
                destroyer(game_buttons, i, j);
            }
        }
    }
}

//usuwanie scierzek nie polonczonych z poczotkiem
void Game::destroyer(Button*** game_buttons, int y, int x) {
    for (int i = 0; i < bx; i++) {
        for (int j = 0; j < by; j++) {
            if (game_buttons[i][j]->isPath && i != 0) {
                game_buttons[i][j]->endPath = true;
        }
        }
    }

    //minimalizowanie b³êdu przy efekcie domina
    for (int j = 0; j < by; j++) {
        for (int i = 0; i < bx; i++) {
            if (game_buttons[i][j]->isPath && !game_buttons[i][j]->endPath) {
                if (j - 1 >= 0) { game_buttons[i][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i - 1 >= 0) { game_buttons[i - 1][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i + 1 < bx) { game_buttons[i + 1][j - 1]->endPath = false; }

                if (i - 1 >= 0) { game_buttons[i - 1][j]->endPath = false; }
                if (i + 1 < bx) { game_buttons[i + 1][j]->endPath = false; }

                if (j + 1 < by) { game_buttons[i][j + 1]->endPath = false; }
                if (j + 1 < by && i - 1 >= 0) { game_buttons[i - 1][j + 1]->endPath = false; }
                if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->endPath = false; }
            }
        }
    }

    for (int i = 0; i < bx; i++) {
        for (int j = 0; j < by; j++) {
            if (game_buttons[i][j]->isPath && !game_buttons[i][j]->endPath) {
                if (j - 1 >= 0) { game_buttons[i][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i - 1 >= 0) { game_buttons[i - 1][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i + 1 < bx) { game_buttons[i + 1][j - 1]->endPath = false; }

                if (i - 1 >= 0) { game_buttons[i - 1][j]->endPath = false; }
                if (i + 1 < bx) { game_buttons[i + 1][j]->endPath = false; }

                if (j + 1 < by) { game_buttons[i][j + 1]->endPath = false; }
                if (j + 1 < by && i - 1 >= 0) { game_buttons[i - 1][j + 1]->endPath = false; }
                if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->endPath = false; }
            }
        }
    }

    for (int i = bx-1; i >= 0; i--) {
        for (int j = by-1; j >= 0; j--) {
            if (game_buttons[i][j]->isPath && !game_buttons[i][j]->endPath) {
                if (j - 1 >= 0) { game_buttons[i][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i - 1 >= 0) { game_buttons[i - 1][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i + 1 < bx) { game_buttons[i + 1][j - 1]->endPath = false; }

                if (i - 1 >= 0) { game_buttons[i - 1][j]->endPath = false; }
                if (i + 1 < bx) { game_buttons[i + 1][j]->endPath = false; }

                if (j + 1 < by) { game_buttons[i][j + 1]->endPath = false; }
                if (j + 1 < by && i - 1 >= 0) { game_buttons[i - 1][j + 1]->endPath = false; }
                if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->endPath = false; }
            }
        }
    }

    for (int j = by-1; j >= 0; j--) {
        for (int i = bx-1; i >= 0; i--) {
            if (game_buttons[i][j]->isPath && !game_buttons[i][j]->endPath) {
                if (j - 1 >= 0) { game_buttons[i][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i - 1 >= 0) { game_buttons[i - 1][j - 1]->endPath = false; }
                if (j - 1 >= 0 && i + 1 < bx) { game_buttons[i + 1][j - 1]->endPath = false; }

                if (i - 1 >= 0) { game_buttons[i - 1][j]->endPath = false; }
                if (i + 1 < bx) { game_buttons[i + 1][j]->endPath = false; }

                if (j + 1 < by) { game_buttons[i][j + 1]->endPath = false; }
                if (j + 1 < by && i - 1 >= 0) { game_buttons[i - 1][j + 1]->endPath = false; }
                if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->endPath = false; }
            }
        }
    }

    for (int j = 0; j < by; j++) {
        for (int i = 1; i < bx; i++) {
            if (game_buttons[i][j]->endPath) {
                game_buttons[i][j]->buttonTexture = IMG_LoadTexture(renderer, "");
                game_buttons[i][j]->buttonHoverTexture = IMG_LoadTexture(renderer, "");
                game_buttons[i][j]->isPath = false;
                game_buttons[i][j]->endPath = false;
                game_buttons[i][j]->canPath = false;
                if (j - 1 >= 0) { game_buttons[i][j - 1]->canPath = false; }
                if (j - 1 >= 0 && i - 1 > 0) { game_buttons[i - 1][j - 1]->canPath = false; }
                if (j - 1 >= 0 && i + 1 < bx) { game_buttons[i + 1][j - 1]->canPath = false; }

                if (i - 1 > 0) { game_buttons[i - 1][j]->canPath = false; }
                if (i + 1 < bx) { game_buttons[i + 1][j]->canPath = false; }

                if (j + 1 < by) { game_buttons[i][j + 1]->canPath = false; }
                if (j + 1 < by && i - 1 > 0) { game_buttons[i - 1][j + 1]->canPath = false; }
                if (j + 1 < by && i + 1 < bx) { game_buttons[i + 1][j + 1]->canPath = false; }
            }
        }
    }
    game_buttons[y][x]->canPath = true;
}

void Game::run(SDL_Texture* gameBackground) {
    isRunning = true; // Reset flagi na pocz¹tku gry
    SDL_Event e;

    //czas
    Uint32 startTime = SDL_GetTicks(); // Start the timer
    Uint32 elapsedTime = 0;


    //wczytywanie tekstó przycisków gry
    int box = 30;
    SDL_Texture* path = IMG_LoadTexture(renderer, "zdj/path.jpg");
    SDL_Texture* h_path = IMG_LoadTexture(renderer, "zdj/hpath.jpg");

    Button*** game_buttons = new Button** [bx];
    for (int i = 0; i < bx; ++i) {
        game_buttons[i] = new Button * [by];
    }

    for (int i = 0; i < bx; i++) {
        for (int j = 0; j < by; j++) {
            game_buttons[i][j] = new Button(renderer, "zdj/n.jpg", "zdj/np.jpg", startX + i * box, startY + j * box, box, box);
        }
    }

    while (isRunning) {
        while (SDL_PollEvent(&e)) {
            
            if (e.type == SDL_QUIT) {
                isRunning = false;
                return; // Wyjœcie z gry
            }
            buttons(e, game_buttons, path, h_path);
        }


        SDL_RenderClear(renderer);

        // Renderowanie t³a gry
        SDL_RenderCopy(renderer, gameBackground, nullptr, nullptr);


        renderTime(startTime);


        //wywo³anie renderowania mapy
        renderBombMap(bombMap, bx, by);

        for (int i = 0; i < bx; i++) {
            for (int j = 0; j < by; j++) {
                game_buttons[i][j]->render(renderer);
            }
        }


        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < bx; ++i) {
        for (int j = 0; j < by; ++j) {
            delete game_buttons[i][j];
        }
        delete[] game_buttons[i];
    }
    delete[] game_buttons;
    SDL_DestroyTexture(path);
    SDL_DestroyTexture(h_path);
    TTF_CloseFont(font);
}

void Game::renderTime(Uint32 startTime) {

  
    // Obliczanie up³ywaj¹cego czasu
    Uint32 elapsedTime = SDL_GetTicks() - startTime;
    std::string timeText = "Czas: " + std::to_string(elapsedTime / 1000) + "s";

    if (stoptime) { 
        //przeniesione do SDLnauka WINSCREAN
        ofstream zapis("zapis/czas.txt", ios::out);
        
        if (zapis.is_open()) {
            // Zapis do pliku
            zapis << elapsedTime / 1000 << "/" << lvl;

            zapis.close();

        }
        else {
            std::cerr << "Nie mo¿na otworzyæ pliku do zapisu!" << std::endl;
        }

      //  cout << "CZAS" << elapsedTime / 1000;
    }

    SDL_Color textColor = { 232, 170, 47, 255 };  // Kolor tekstu

    // Tworzenie powierzchni tekstu
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    // Ustawienie wymiary prostok¹ta t³a
    SDL_Rect textRect = { 300, 40, 150, 40 };
    SDL_SetRenderDrawColor(renderer, 167, 227, 255, 255);  // Kolor t³a
    SDL_RenderFillRect(renderer, &textRect);

    SDL_Rect textRenderRect = { textRect.x + 10, textRect.y + 10, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &textRenderRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


bool Game::hasWon() {
    return won;
}
bool Game::hasLost() {
    return lost;
}

