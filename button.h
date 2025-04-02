#pragma once
#include "SDL.h"
#include <string>

class Button {
public:
    SDL_Texture* buttonTexture;    // Normalna tekstura
    SDL_Texture* buttonHoverTexture; // Tekstura po najechaniu
    SDL_Rect buttonRect;           // Obszar przycisku
    bool isHovered;                // Czy mysz najecha�a na przycisk
    bool isClicked;                // Czy przycisk zosta� klikni�ty
    //na potrzeby gry
    bool isRightClicked;            //klikni�cie ppm
    bool wasClicked;                //przycisk by� ju� klikni�ty
    bool isPath;                    //przycisk jest �cieszk�
    bool canPath;                   //przycisk mo�e by� �cieszk�
    bool endPath;                   //warto�� do usuwania ze �cieszki

    Button(SDL_Renderer* renderer, const std::string& texturePath, const std::string& hoverTexturePath, int x, int y, int w, int h);
    ~Button();

    void handleEvent(const SDL_Event& event);
    void render(SDL_Renderer* renderer);
    void changeTexture(SDL_Texture* newTexture, SDL_Texture* hoverTexture);
};
