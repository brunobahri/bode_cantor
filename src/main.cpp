#include "main.hpp"

MusicPlayerApp::MusicPlayerApp() 
    : window(nullptr), renderer(nullptr), buttonTexture(nullptr), imageTexture(nullptr), font(nullptr), music(nullptr), isRunning(true) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    window = SDL_CreateWindow("Music Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL Error: " << SDL_GetError() << std::endl;
        isRunning = false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio failed! Mix Error: " << Mix_GetError() << std::endl;
        isRunning = false;
    }

    initTTF();
    createButton();
    loadImage();
    loadMusic("../assets/musica.mp3");
}

MusicPlayerApp::~MusicPlayerApp() {
    cleanUp();
}

void MusicPlayerApp::initTTF() {
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF Error: " << TTF_GetError() << std::endl;
        isRunning = false;
    }
    font = TTF_OpenFont("../assets/YourFont.otf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF Error: " << TTF_GetError() << std::endl;
        isRunning = false;
    }
}

void MusicPlayerApp::createButton() {
    SDL_Color color = {0, 0, 0};
    SDL_Surface* surface = TTF_RenderText_Solid(font, "Foi ha 37 anos", color);
    buttonTexture = SDL_CreateTextureFromSurface(renderer, surface);
    buttonRect = {320 - surface->w / 2, 240 - surface->h / 2, surface->w, surface->h};
    SDL_FreeSurface(surface);
}

void MusicPlayerApp::loadMusic(const char* filename) {
    music = Mix_LoadMUS(filename);
    if (!music) {
        std::cerr << "Failed to load music! Mix Error: " << Mix_GetError() << std::endl;
    } else {
        std::cout << "Music loaded successfully!" << std::endl;
    }
}

void MusicPlayerApp::loadImage() {
    SDL_Surface* loadedSurface = IMG_Load("../assets/foto.jpg");
    if (!loadedSurface) {
        std::cerr << "Unable to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        isRunning = false;
    } else {
        imageTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (!imageTexture) {
            std::cerr << "Unable to create texture from image! SDL Error: " << SDL_GetError() << std::endl;
        }
        imageRect = {0, 0, loadedSurface->w, loadedSurface->h};
        SDL_FreeSurface(loadedSurface);
    }
}

void MusicPlayerApp::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        } else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (x > buttonRect.x && x < buttonRect.x + buttonRect.w &&
                y > buttonRect.y && y < buttonRect.y + buttonRect.h) {
                Mix_PlayMusic(music, -1);
            }
        }
    }
}

void MusicPlayerApp::render() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);
    imageRect.x = (windowWidth - imageRect.w) / 2;
    imageRect.y = (windowHeight - imageRect.h - buttonRect.h - 50) / 2;

    buttonRect.x = (windowWidth - buttonRect.w) / 2;
    buttonRect.y = imageRect.y + imageRect.h + 50;

    if (imageTexture) {
        SDL_RenderCopy(renderer, imageTexture, NULL, &imageRect);
    }
    if (buttonTexture) {
        SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);
    }
    SDL_RenderPresent(renderer);
}

void MusicPlayerApp::cleanUp() {
    if (imageTexture) SDL_DestroyTexture(imageTexture);
    if (buttonTexture) SDL_DestroyTexture(buttonTexture);
    if (music) Mix_FreeMusic(music);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    if (font) TTF_CloseFont(font);
    TTF_Quit();
    Mix_Quit();
    SDL_Quit();
}

void MusicPlayerApp::run() {
    while (isRunning) {
        handleEvents();
        render();
    }
}

int main() {
    MusicPlayerApp app;
    app.run();
    return 0;
}
