#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
// #include <stdint.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define GAP 30

int main(int argc, char *argv[]) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;

    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intitalize SDL: %s", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Game of Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    texture = SDL_CreateTexture(
                renderer,
                SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STATIC,
                WINDOW_WIDTH,
                WINDOW_HEIGHT
            );

    rect.w = 20;
    rect.h = 100;

    rect.x = 500;
    rect.y = 500;

    // The window is opened


    SDL_Event e;
    while (1) {
        Uint64 start = SDL_GetPerformanceCounter();
        SDL_PollEvent(&e);

        if (e.type == SDL_QUIT) {
            SDL_Log("Program quit after %i ticks", e.quit.timestamp);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 0;
        }

        if (e.type == SDL_KEYDOWN) {
            SDL_Scancode key = e.key.keysym.scancode;

            if (key == SDL_SCANCODE_K && rect.y > GAP) {
                rect.y -= 20;
            }

            else if (key == SDL_SCANCODE_J && rect.y + rect.h + GAP < WINDOW_HEIGHT) {
                rect.y += 20;
            }

            else if (key == SDL_SCANCODE_L && rect.x + rect.w + GAP < WINDOW_WIDTH) {
                rect.x += 20;
            }

            else if (key == SDL_SCANCODE_H && rect.x > GAP) {
                rect.x -= 20;
            }
        }

        SDL_SetRenderTarget(renderer, texture);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
        SDL_RenderFillRect(renderer, &rect);
        SDL_RenderPresent(renderer);

        Uint64 end = SDL_GetPerformanceCounter();

        int fps = 1.0 / ((end - start) / (float) SDL_GetPerformanceFrequency());
        printf("FPS: %i\n", fps);
    }
}
