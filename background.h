#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Background {
private:
    SDL_Renderer* renderer;
    SDL_Texture* currentTexture;  // Tekstura t³a

public:
    Background(SDL_Renderer* rend, const char* initialBackgroundPath);
    ~Background();

    void setBackground(const char* texturePath);  // Ustawienie t³a
    void render();  // Renderowanie t³a

    SDL_Texture* getTexture() const;  // Funkcja do zwracania tekstury t³a
};
