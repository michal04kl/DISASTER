#include "button.h"
#include "SDL_image.h"
#include <iostream>

Button::Button(SDL_Renderer* renderer, const std::string& texturePath, const std::string& hoverTexturePath, int x, int y, int w, int h) {
    // Wczytanie tekstur
    buttonTexture = IMG_LoadTexture(renderer, texturePath.c_str());
    buttonHoverTexture = IMG_LoadTexture(renderer, hoverTexturePath.c_str());

    if (!buttonTexture || !buttonHoverTexture) {
        std::cerr << "Nie mo¿na wczytaæ tekstur przycisku: " << SDL_GetError() << std::endl;
    }

    buttonRect = { x, y, w, h };
    isHovered = false;
    isClicked = false;
    isRightClicked = false;
    wasClicked = false;
    isPath = false;
    canPath = false;
    endPath = false;
}

Button::~Button() {
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyTexture(buttonHoverTexture);
}

void Button::handleEvent(const SDL_Event& event) {
    // Sprawdzenie pozycji myszy
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    SDL_Point mousePoint = { mouseX, mouseY };
    isHovered = SDL_PointInRect(&mousePoint, &buttonRect);

    if (event.type == SDL_MOUSEBUTTONDOWN && isHovered) {
        if (event.button.button == SDL_BUTTON_LEFT) {
            isClicked = true;
        }
        else if (event.button.button == SDL_BUTTON_RIGHT) {
            isRightClicked = !isRightClicked;
        }
    }
}

void Button::render(SDL_Renderer* renderer) {
    if (isHovered) {
        SDL_RenderCopy(renderer, buttonHoverTexture, NULL, &buttonRect);
    }
    else {
        SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);
    }
}

//zmiana tesktury przycsików
void Button::changeTexture(SDL_Texture* newTexture, SDL_Texture* hoverTexture) {
    if (isClicked || isRightClicked) {
        if (buttonTexture) {
            SDL_DestroyTexture(buttonTexture);
        }
        if (buttonHoverTexture) {
            SDL_DestroyTexture(buttonHoverTexture);
        }
        buttonTexture = newTexture;
        buttonHoverTexture = hoverTexture;
    }
}
