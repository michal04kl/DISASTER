#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include <iostream>

class Background {
private:
    SDL_Renderer* renderer;
    SDL_Texture* currentTexture;  // Tekstura t�a

public:
    Background(SDL_Renderer* rend, const char* initialBackgroundPath);
    ~Background();

    void setBackground(const char* texturePath);  // Ustawienie t�a
    void render();  // Renderowanie t�a

    SDL_Texture* getTexture() const;  // Funkcja do zwracania tekstury t�a
};
