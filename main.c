#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define GAP 10

int main(int argc, char *argv[]) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect rect;

    typedef struct {
        bool h_pressed;
        bool j_pressed;
        bool k_pressed;
        bool l_pressed;
    }
    keys;

    keys kb;

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

    renderer = SDL_CreateRenderer(
                    window,
                    -1,
                    SDL_RENDERER_PRESENTVSYNC | 
                    SDL_RENDERER_ACCELERATED
                );

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

        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_Log("Program quit after %i ticks", e.quit.timestamp);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }

            SDL_Scancode key = e.key.keysym.scancode;

            if (e.type == SDL_KEYDOWN) {

                if (key == SDL_SCANCODE_K) {
                    kb.k_pressed = true;
                }

                else if (key == SDL_SCANCODE_J) {
                    kb.j_pressed = true;
                }

                else if (key == SDL_SCANCODE_L) {
                    kb.l_pressed = true;
                }

                else if (key == SDL_SCANCODE_H) {
                    kb.h_pressed = true;
                }
            }

            if (e.type == SDL_KEYUP) {

                if (key == SDL_SCANCODE_K) {
                    kb.k_pressed = false;
                }

                else if (key == SDL_SCANCODE_J) {
                    kb.j_pressed = false;
                }

                else if (key == SDL_SCANCODE_L) {
                    kb.l_pressed = false;
                }

                else if (key == SDL_SCANCODE_H) {
                    kb.h_pressed = false;
                }
            }

            if (kb.k_pressed && rect.y > GAP) {
                    rect.y -= 10;
            }

            else if (kb.j_pressed && rect.y + rect.h + GAP < WINDOW_HEIGHT) {
                    rect.y += 10;
            }

            else if (kb.l_pressed && rect.x + rect.w + GAP < WINDOW_WIDTH) {
                    rect.x += 10;
            }

            else if (kb.h_pressed && rect.x > GAP) {
                    rect.x -= 10;
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
