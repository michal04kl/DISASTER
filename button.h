#pragma once
#include "SDL.h"
#include <string>

class Button {
public:
    SDL_Texture* buttonTexture;    // Normalna tekstura
    SDL_Texture* buttonHoverTexture; // Tekstura po najechaniu
    SDL_Rect buttonRect;           // Obszar przycisku
    bool isHovered;                // Czy mysz najecha³a na przycisk
    bool isClicked;                // Czy przycisk zosta³ klikniêty
    //na potrzeby gry
    bool isRightClicked;            //klikniêcie ppm
    bool wasClicked;                //przycisk by³ ju¿ klikniêty
    bool isPath;                    //przycisk jest œcieszk¹
    bool canPath;                   //przycisk mo¿e byæ œcieszk¹
    bool endPath;                   //wartoœæ do usuwania ze œcieszki

    Button(SDL_Renderer* renderer, const std::string& texturePath, const std::string& hoverTexturePath, int x, int y, int w, int h);
    ~Button();

    void handleEvent(const SDL_Event& event);
    void render(SDL_Renderer* renderer);
    void changeTexture(SDL_Texture* newTexture, SDL_Texture* hoverTexture);
};
