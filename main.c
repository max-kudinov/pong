#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define RECT_WIDTH 20
#define RECT_HEIGHT 100

#define LINE_WIDTH 10
#define LINE_HEIGHT 50

#define RECT_SPEED 25

struct {
    bool j_pressed;
    bool k_pressed;
    bool up_pressed;
    bool down_pressed;
} key_status;

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect player_paddle;
    SDL_Rect computer_paddle;
    SDL_Rect line;
} display;

void detect_keys(SDL_Scancode scancode, bool pressed);
int handle_input(SDL_Event *event);
void render_frame();

int main(int argc, char *argv[]) {
    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't intitalize SDL: %s", SDL_GetError());
        return 1;
    }

    display.window = SDL_CreateWindow(
        "Game of Pong",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL |
        SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (display.window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    display.renderer = SDL_CreateRenderer(
                    display.window,
                    -1,
                    SDL_RENDERER_PRESENTVSYNC | 
                    SDL_RENDERER_ACCELERATED
                );

    display.texture = SDL_CreateTexture(
                    display.renderer,
                    SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_STATIC,
                    WINDOW_WIDTH,
                    WINDOW_HEIGHT
                );

    display.player_paddle.w = RECT_WIDTH;
    display.player_paddle.h = RECT_HEIGHT;
    display.player_paddle.x = WINDOW_WIDTH - 70;
    display.player_paddle.y = WINDOW_HEIGHT / 2 - RECT_HEIGHT / 2;

    display.computer_paddle.w = RECT_WIDTH;
    display.computer_paddle.h = RECT_HEIGHT;
    display.computer_paddle.x = 70 - RECT_WIDTH;
    display.computer_paddle.y = WINDOW_HEIGHT / 2 - RECT_HEIGHT / 2;

    display.line.w = LINE_WIDTH;
    display.line.h = LINE_HEIGHT;
    display.line.x = WINDOW_WIDTH / 2 - LINE_WIDTH / 2;

    SDL_Event event;

    while (1) {

        if (handle_input(&event)) {
            return 1;
        }

        if ((key_status.k_pressed || key_status.up_pressed) && display.player_paddle.y > 0) {
            display.player_paddle.y -= RECT_SPEED;
            display.computer_paddle.y -= RECT_SPEED;
        }

        else if ((key_status.j_pressed || key_status.down_pressed) &&
                  display.player_paddle.y + RECT_HEIGHT < WINDOW_HEIGHT) {
            display.player_paddle.y += RECT_SPEED;
            display.computer_paddle.y += RECT_SPEED;
        }

        render_frame();
    }
}

void detect_keys(SDL_Scancode scancode, bool pressed) {
    switch (scancode) {
        case SDL_SCANCODE_UP:
            key_status.up_pressed = pressed;
            break;

        case SDL_SCANCODE_DOWN:
            key_status.down_pressed = pressed;
            break;

        case SDL_SCANCODE_J:
            key_status.j_pressed = pressed;
            break;

        case SDL_SCANCODE_K:
            key_status.k_pressed = pressed;
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
                SDL_DestroyRenderer(display.renderer);
                SDL_DestroyWindow(display.window);
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

void render_frame() {
    // Render background
    SDL_SetRenderTarget(display.renderer, display.texture);
    SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 0);
    SDL_RenderClear(display.renderer);

    // Render paddles
    SDL_SetRenderDrawColor(display.renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(display.renderer, &display.player_paddle);
    SDL_RenderFillRect(display.renderer, &display.computer_paddle);

    // Render center line
    for (int i = 0; i < WINDOW_HEIGHT; i += LINE_HEIGHT * 1.5) {
        display.line.y = i;
        SDL_RenderFillRect(display.renderer, &display.line);
    }

    SDL_RenderPresent(display.renderer);
}
