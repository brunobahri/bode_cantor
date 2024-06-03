#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

class MusicPlayerApp {
public:
    MusicPlayerApp();
    ~MusicPlayerApp();
    void run();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buttonTexture;
    SDL_Texture* imageTexture;  // Textura para a imagem
    SDL_Rect buttonRect;
    SDL_Rect imageRect;  // Retângulo para a imagem
    TTF_Font* font;
    Mix_Music* music;
    bool isRunning;

    void initSDL();
    void initTTF();
    void loadMusic(const char* filename);
    void createButton();
    void loadImage();  // Função para carregar a imagem
    void handleEvents();
    void render();
    void cleanUp();
};
