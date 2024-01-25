#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define RECT_WIDTH 20
#define RECT_HEIGHT 100

#define RECT_SPEED 25

#define GAP 0

typedef struct {
    bool j_pressed;
    bool k_pressed;
    bool up_pressed;
    bool down_pressed;
} key_status;

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect player_rect;
    SDL_Rect computer_rect;
    key_status keys;
} state;

void detect_keys(SDL_Scancode scancode, bool pressed);
int handle_input(SDL_Event *event);
void render();

int main(int argc, char *argv[]) {
    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intitalize SDL: %s", SDL_GetError());
        return 1;
    }

    state.window = SDL_CreateWindow(
        "Game of Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (state.window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    state.renderer = SDL_CreateRenderer(
                    state.window,
                    -1,
                    SDL_RENDERER_PRESENTVSYNC | 
                    SDL_RENDERER_ACCELERATED
                );

    state.texture = SDL_CreateTexture(
                    state.renderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_STATIC,
                    WINDOW_WIDTH,
                    WINDOW_HEIGHT
                );

    state.player_rect.w = 20;
    state.player_rect.h = 100;

    state.player_rect.x = WINDOW_WIDTH - 70;
    state.player_rect.y = WINDOW_HEIGHT / 2 - RECT_HEIGHT / 2;

    SDL_Event event;

    while (1) {

        if (handle_input(&event)) {
            return 1;
        }

        if ((state.keys.k_pressed || state.keys.up_pressed) && state.player_rect.y > 0) {
            state.player_rect.y -= RECT_SPEED;
        }

        else if ((state.keys.j_pressed || state.keys.down_pressed) &&
                  state.player_rect.y + RECT_HEIGHT < WINDOW_HEIGHT) {
            state.player_rect.y += RECT_SPEED;
        }
        render();
    }
}

void detect_keys(SDL_Scancode scancode, bool pressed) {
    switch (scancode) {
        case SDL_SCANCODE_UP:
            state.keys.up_pressed = pressed;
            break;

        case SDL_SCANCODE_DOWN:
            state.keys.down_pressed = pressed;
            break;

        case SDL_SCANCODE_J:
            state.keys.j_pressed = pressed;
            break;

        case SDL_SCANCODE_K:
            state.keys.k_pressed = pressed;
            break;

        default:
            break;
    }
}

int handle_input(SDL_Event *event) {

    while(SDL_PollEvent(event)) {

        SDL_Scancode scancode = event->key.keysym.scancode;

        switch(event->type) {
            case SDL_QUIT:
                SDL_Log("Program quit after %i ticks", event->quit.timestamp);
                SDL_DestroyRenderer(state.renderer);
                SDL_DestroyWindow(state.window);
                SDL_Quit();
                return 1;

            case SDL_KEYDOWN:
                detect_keys(scancode, true);
                break;

            case SDL_KEYUP:
                detect_keys(scancode, false);
                break;
        }
    }
    return 0;
}

void render() {
    SDL_SetRenderTarget(state.renderer, state.texture);
    SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 0);
    SDL_RenderClear(state.renderer);
    SDL_RenderDrawRect(state.renderer, &state.player_rect);
    SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(state.renderer, &state.player_rect);
    SDL_RenderPresent(state.renderer);
}
