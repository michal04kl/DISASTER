#include "background.h"

Background::Background(SDL_Renderer* rend, const char* initialBackgroundPath)
    : renderer(rend), currentTexture(nullptr) {
    setBackground(initialBackgroundPath);  // Ustawienie pocz¹tkowego t³a
}

Background::~Background() {
    if (currentTexture) {
        SDL_DestroyTexture(currentTexture);  // Zwolnienie tekstury
    }
}

void Background::setBackground(const char* texturePath) {
    if (currentTexture) {
        SDL_DestroyTexture(currentTexture);  // Zwalniamy star¹ teksturê
    }
    currentTexture = IMG_LoadTexture(renderer, texturePath);  // £adowanie nowej tekstury

    // Sprawdzamy, czy za³adowano teksturê
    if (!currentTexture) {
        std::cerr << "Failed to load background texture: " << IMG_GetError() << std::endl;
    }
}

void Background::render() {
    if (currentTexture) {
        SDL_RenderCopy(renderer, currentTexture, nullptr, nullptr);  // Renderowanie t³a
    }
}

SDL_Texture* Background::getTexture() const {
    return currentTexture;  // Zwracamy teksturê t³a
}
