#include "SDL.h"
#include "SDL_image.h"
#include "background.h"
#include "button.h"
#include "Game.h"
#include <iostream>
#include <windows.h>
#include "SDL_ttf.h"
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <sstream>
#include <vector>
using namespace std;


struct Wynik {
    string nazwa_uzytkownika;
    int czas;
    int poziom;
};


void wyswietl_top_wyniki(const vector<string>& wyniki, TTF_Font* font, SDL_Renderer* renderer);

bool porownaj(const Wynik& a, const Wynik& b) {
    return a.czas < b.czas;
}



int CALLBACK WinMain(
    __in  HINSTANCE hInstance,
    __in  HINSTANCE hPrevInstance,
    __in  LPSTR lpCmdLine,
    __in  int nCmdShow
) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    SDL_StartTextInput();
    TTF_Font* font = TTF_OpenFont("czcionki/BubbleGarden.ttf", 256);
    TTF_Font* font1 = TTF_OpenFont("czcionki/BubbleGarden.ttf", 60);
    std::string playerName = "";
    SDL_Color textColor = { 255, 255, 255, 255 };

    enum GameState {
        START_SCREEN,
        ENTER_NAME,
        MENU,
        PLAY_GAME,
        GAME_OVER,
        WIN_SCREEN,
        LOST_SCREEN,
        TABLE_SCORE
    };

   

    int currentLevel = 0;
    SDL_Renderer* renderer = nullptr;
    SDL_Window* window = nullptr;
 

    window = SDL_CreateWindow("Menu Gry", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 750, 570, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    Background background(renderer, "zdj/menu.jpg");

    Button startButton(renderer, "zdj/buttons.jpg", "zdj/hbuttons.jpg", 240, 350, 270, 60);
    Button playButton(renderer, "zdj/buttonp.jpg", "zdj/hbuttonp.jpg", 240, 350, 270, 60);
    Button button1(renderer, "zdj/button1.jpg", "zdj/hbutton1.jpg", 70, 270, 270, 60);
    Button button2(renderer, "zdj/button2.jpg", "zdj/hbutton2.jpg", 409, 270, 270, 60);
    Button button3(renderer, "zdj/button3.jpg", "zdj/hbutton3.jpg", 70, 400, 270, 60);
    Button button4(renderer, "zdj/buttonw.jpg", "zdj/hbuttonw.jpg", 409, 400, 270, 60);
    Button backButton(renderer, "zdj/buttonm.jpg", "zdj/hbuttonm.jpg", 240, 500, 270, 60);

    GameState state = START_SCREEN;
    int unlockedButtons = 0;
    bool isRunning = true;

    SDL_Rect textRect = { 240, 300, 270, 40 };
    SDL_Texture* gameBackground = nullptr;
    SDL_Texture* textTexture = nullptr;
    SDL_Texture* winTexture = IMG_LoadTexture(renderer, "zdj/wygrana.jpg");
    SDL_Texture* lostTexture = IMG_LoadTexture(renderer, "zdj/przegrana.jpg");
    const char* level1Backgrounds[] = { "zdj/zdjm3.jpg", "zdj/zdjm1.jpg", "zdj/zdjm2.jpg" };
    const char* level2and3Backgrounds[] = { "zdj/zdjd3.jpg", "zdj/zdjd4.jpg", "zdj/zdjd1.jpg", "zdj/zdjd2.jpg" };

    while (isRunning) {
        SDL_Event event;
        SDL_Surface* textSurface = nullptr;  // Deklaracja zmiennej przed pętlą zdarzeń

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::ofstream zapis("zapis/czas.txt");
                zapis.close();
                isRunning = false;
            }

            switch (state) {
            case START_SCREEN:
                startButton.handleEvent(event);
                if (startButton.isClicked) {
                    state = ENTER_NAME;
                    startButton.isClicked = false;
                }
                break;

            case ENTER_NAME:
                playButton.handleEvent(event);
                if (event.type == SDL_TEXTINPUT) {
                    playerName += event.text.text;
                }
                else if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0) {
                        playerName.pop_back();
                    }
                }
                if (playButton.isClicked) {
                    

                    std::ofstream zapis("zapis/gracze.txt", std::ios::app);  // Otwiera plik do zapisu
                    zapis.is_open();
                    if (zapis.is_open()) {
                        zapis <<std::endl<< playerName; // Zapisz tekst do pliku
                        zapis.close();  // Zamknij plik po zapisaniu
                    }
                    else {
                        std::cout << "Błąd podczas otwierania pliku!" << std::endl;
                    }


                    state = MENU;
                    //unlockedButtons = 1;
                    std::ifstream lvl("zapis/lvl.txt");
                    if (!lvl.is_open() || lvl.fail()) {
                        unlockedButtons = 1;
                    }
                    else {
                        lvl >> unlockedButtons;
                    }
                    lvl.close();
                    playButton.isClicked = false;
                }
                break;

            case MENU:

                if (unlockedButtons >= 1) button1.handleEvent(event);
                if (unlockedButtons >= 2) button2.handleEvent(event);
                if (unlockedButtons >= 3) button3.handleEvent(event);
                if (unlockedButtons >= 1) button4.handleEvent(event);

                if (button1.isClicked) {
                   
                    srand(time(nullptr));
                    currentLevel = 1;
                    state = PLAY_GAME;
                    const char* randomBackground = level1Backgrounds[rand() % 3];
                    background.setBackground(randomBackground);
                    gameBackground = background.getTexture();
                    button1.isClicked = false;
                } else if (button2.isClicked && unlockedButtons >= 2) {
                    currentLevel = 2;
                    state = PLAY_GAME;
                    const char* randomBackground = level2and3Backgrounds[rand() % 4];
                    background.setBackground(randomBackground);
                    gameBackground = background.getTexture();
                    button2.isClicked = false;
                } else if (button3.isClicked && unlockedButtons >= 3) {
                    currentLevel = 3;
                    state = PLAY_GAME;
                    const char* randomBackground = level2and3Backgrounds[rand() % 4];
                    background.setBackground(randomBackground);
                    gameBackground = background.getTexture();
                    button3.isClicked = false;
                } else if (button4.isClicked && unlockedButtons >= 1) {
                    
                    state = TABLE_SCORE;
                    
                    button4.isClicked = false;
                }
                break;
            case TABLE_SCORE:
                backButton.handleEvent(event);
                if (backButton.isClicked) {
                    state = MENU;
                    backButton.isClicked = false;
                }
                break;
            default:
                break;
            }
        }

        SDL_RenderClear(renderer);

        switch (state) {
        case START_SCREEN:
            background.setBackground("zdj/start.jpg");
            background.render();
            startButton.render(renderer);
            break;
        case ENTER_NAME:
            playButton.handleEvent(event);

            if (event.type == SDL_TEXTINPUT) {
                playerName += event.text.text;  // Dodaj tekst wpisany przez użytkownika

            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_BACKSPACE && playerName.length() > 0) {
                    playerName.pop_back();  // Usuń ostatni znak
                }
            }

            if (playButton.isClicked) {


                state = MENU;
                unlockedButtons = 1;
                playButton.isClicked = false;


            }

            background.setBackground("zdj/menu.jpg");
            background.render();

            // Generowanie nowej tekstury przy każdej zmianie tekstu
            textSurface = TTF_RenderText_Solid(font, playerName.c_str(), textColor);
            if (textSurface != nullptr) {
                if (textTexture != nullptr) {
                    SDL_DestroyTexture(textTexture);  // Usuwamy starą teksturę
                }
                textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                SDL_FreeSurface(textSurface);  // Zwolnij pamięć po stworzeniu tekstury
            }

            // Rysowanie tła prostokąta
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            SDL_SetRenderDrawColor(renderer, 167, 227, 255, 170);
            SDL_RenderFillRect(renderer, &textRect);  // Tło dla tekstu
            SDL_SetRenderDrawColor(renderer, 255, 2, 221, 255);

            // Rysowanie tekstu
            if (textTexture != nullptr) {
                int textWidth = 0;
                TTF_SizeText(font1, playerName.c_str(), &textWidth, nullptr);  // Pobierz szerokość tekstu

                // Ustalamy szerokość tekstu na maxWidth, jeśli jest za szeroki
                if (textWidth > textRect.w) {
                    textWidth = textRect.w;
                }

                SDL_Rect textRectUpdated = { textRect.x, textRect.y, textWidth, textRect.h };
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRectUpdated);  // Rysowanie tekstu
            }


            playButton.render(renderer);
            break;





        case MENU:


            background.setBackground("zdj/menu.jpg");
            background.render();
            if (unlockedButtons >= 1) button1.render(renderer);
            if (unlockedButtons >= 2) button2.render(renderer);
            if (unlockedButtons >= 3) button3.render(renderer);
            if (unlockedButtons >= 1) button4.render(renderer);
            break;

        case PLAY_GAME:
        {
            Game game(renderer, currentLevel);
            game.run(gameBackground);
            if (game.hasWon()) {
                state = WIN_SCREEN;
            }
            else if (game.hasLost()) {
                state = LOST_SCREEN;
            }
            else {
                isRunning = false;
            }
        }
        break;

        case WIN_SCREEN:{
            Sleep(1000);
            SDL_RenderCopy(renderer, winTexture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_Delay(1500);
            state = MENU;
            if (currentLevel == unlockedButtons && unlockedButtons != 3) {
                unlockedButtons++;
                std::fstream lvl("zapis/lvl.txt", std::ios::out);
                if (lvl.is_open() && !lvl.fail()) {
                    lvl << unlockedButtons;
                }
                lvl.close();
            }
            std::ofstream zapis("zapis/tabela.txt", std::ios::app);
            std::ifstream wyniki("zapis/czas.txt", std::ios::in);

            if (zapis.is_open()&&wyniki.is_open()) {
                // Zapis do pliku
                std::string l;
                wyniki >> l;

                zapis << playerName << "/" << l << std::endl;

                zapis.close();
                wyniki.close();
            }
            else {
                std::cerr << "Nie można otworzyć pliku do zapisu!" << std::endl;
            }

            break;
        }
        case LOST_SCREEN:
            Sleep(1000);
            SDL_RenderCopy(renderer, lostTexture, nullptr, nullptr);
            SDL_RenderPresent(renderer);
            SDL_Delay(1500);
            state = MENU;
            break;

        case TABLE_SCORE:
        {
            // Załaduj dane z pliku wyników
            std::ifstream wynikiFile("zapis/tabela.txt");
            std::vector<std::string> wyniki;
            std::string line;

            // Wczytanie wszystkich wyników
            while (std::getline(wynikiFile, line)) {
                wyniki.push_back(line);
            }
            wynikiFile.close();

           

            // Wyświetlanie najlepszych wyników
            background.setBackground("zdj/menu.jpg");
            background.render();


            backButton.render(renderer);
            backButton.handleEvent(event);
          

            wyswietl_top_wyniki(wyniki, font, renderer);  // Przekazanie fonta i renderera

            SDL_RenderPresent(renderer);  // Wyświetlenie na ekranie
            break;
        }



        default:
            break;
        }

        SDL_RenderPresent(renderer);
    }

    if (textTexture != nullptr) {
        SDL_DestroyTexture(textTexture);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}




void wyswietl_top_wyniki(const vector<string>& wyniki, TTF_Font* font, SDL_Renderer* renderer) {
    vector<Wynik> dane;

    // Przekształcenie danych wejściowych na struktury
    for (const string& wynik : wyniki) {
        stringstream ss(wynik);
        string nazwa_uzytkownika, czas_str, poziom_str;
        getline(ss, nazwa_uzytkownika, '/');
        getline(ss, czas_str, '/');
        getline(ss, poziom_str);

        Wynik w;
        w.nazwa_uzytkownika = nazwa_uzytkownika;
        w.czas = stoi(czas_str);
        w.poziom = stoi(poziom_str);
        dane.push_back(w);
    }

    // Sortowanie wyników po czasie
    sort(dane.begin(), dane.end(), porownaj);

    // Mapa do przechowywania najlepszych wyników dla poziomów 1, 2, 3
    map<int, Wynik> najlepsze_wyniki;

    // Wybieranie najlepszych wyników dla każdego poziomu
    for (const Wynik& wynik : dane) {
        if (najlepsze_wyniki.find(wynik.poziom) == najlepsze_wyniki.end()) {
            najlepsze_wyniki[wynik.poziom] = wynik;
        }
    }

    // Rysowanie wyników na ekranie
    int yOffset = 200;

    //ramka
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 167, 227, 255, 170); // Jasnoniebieski z półprzezroczystością
    SDL_Rect borderRect = { 40, yOffset - 10, 670, 260};
    SDL_RenderFillRect(renderer, &borderRect); // Rysowanie prostokąta z półprzezroczystością


    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Najlepsze wyniki poziomow:", { 255, 255, 255, 255 });
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    SDL_FreeSurface(titleSurface);

    SDL_Rect titleRect = { 50, yOffset, 650, 40 };
    SDL_RenderCopy(renderer, titleTexture, nullptr, &titleRect);
    SDL_DestroyTexture(titleTexture);

    yOffset += 75;

    for (int poziom = 1; poziom <= 3; ++poziom) {
        if (najlepsze_wyniki.find(poziom) != najlepsze_wyniki.end()) {
            const Wynik& wynik = najlepsze_wyniki[poziom];

            stringstream ss;
            ss << "Poziom " << poziom << ":     " << wynik.nazwa_uzytkownika << " - " << wynik.czas << " sekund";
            string wynikText = ss.str();

            SDL_Surface* textSurface = TTF_RenderText_Solid(font, wynikText.c_str(), { 255, 255, 255, 255 });
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect = { 50, yOffset, 650, 40 };
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            SDL_DestroyTexture(textTexture);

            yOffset += 50;
        }
    }

}
