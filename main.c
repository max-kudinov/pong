#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100

#define LINE_WIDTH 10
#define LINE_HEIGHT 50

#define BALL_SIDE 25
#define BALL_INIT_SPEED 15

#define PADDLE_SPEED 25

struct {
    bool j_pressed;
    bool k_pressed;
    bool up_pressed;
    bool down_pressed;
} key_status;

struct {
    int speed_x;
    int speed_y;
} ball_stat;

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect player_paddle;
    SDL_Rect computer_paddle;
    SDL_Rect line;
    SDL_Rect ball;
} display;

int init_window();
void objects_setup();
void render_frame();
int handle_input(SDL_Event *event);
void move_ball();
void computer_move();
void detect_keys(SDL_Scancode scancode, bool pressed);
bool is_colliding(SDL_Rect paddle, SDL_Rect ball);

int main(int argc, char *argv[]) {
    SDL_Event event;

    if (init_window()) {
        return 1;
    }

    objects_setup();

    while (1) {
        if (handle_input(&event)) {
            return 1;
        }

        if ((key_status.k_pressed || key_status.up_pressed) &&
            display.player_paddle.y > 0) {
            display.player_paddle.y -= PADDLE_SPEED;
            display.computer_paddle.y -= PADDLE_SPEED;
        }

        else if ((key_status.j_pressed || key_status.down_pressed) &&
                 display.player_paddle.y + PADDLE_HEIGHT < WINDOW_HEIGHT) {
            display.player_paddle.y += PADDLE_SPEED;
            display.computer_paddle.y += PADDLE_SPEED;
        }

        move_ball();
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

    while (SDL_PollEvent(event)) {

        SDL_Scancode scancode = event->key.keysym.scancode;

        switch (event->type) {
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

    // Render ball
    SDL_RenderFillRect(display.renderer, &display.ball);

    // Render center line
    for (int i = 0; i < WINDOW_HEIGHT; i += LINE_HEIGHT * 1.5) {
        display.line.y = i;
        SDL_RenderFillRect(display.renderer, &display.line);
    }

    SDL_RenderPresent(display.renderer);
}

int init_window() {

    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't intitalize SDL: %s", SDL_GetError());
        return 1;
    }

    display.window =
        SDL_CreateWindow("Game of Pong", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    if (display.window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    display.renderer = SDL_CreateRenderer(display.window, -1,
                                          SDL_RENDERER_PRESENTVSYNC |
                                              SDL_RENDERER_ACCELERATED);

    display.texture = SDL_CreateTexture(
        display.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    return 0;
}

void objects_setup() {
    display.player_paddle.w = PADDLE_WIDTH;
    display.player_paddle.h = PADDLE_HEIGHT;
    display.player_paddle.x = WINDOW_WIDTH - 70;
    display.player_paddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;

    display.computer_paddle.w = PADDLE_WIDTH;
    display.computer_paddle.h = PADDLE_HEIGHT;
    display.computer_paddle.x = 70 - PADDLE_WIDTH;
    display.computer_paddle.y = WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2;

    display.line.w = LINE_WIDTH;
    display.line.h = LINE_HEIGHT;
    display.line.x = WINDOW_WIDTH / 2 - LINE_WIDTH / 2;

    display.ball.w = BALL_SIDE;
    display.ball.h = BALL_SIDE;

    display.ball.x = WINDOW_WIDTH / 2 - BALL_SIDE / 2;
    display.ball.y = WINDOW_HEIGHT / 2;

    ball_stat.speed_x = BALL_INIT_SPEED;
    ball_stat.speed_y = 0;
}

bool is_colliding(SDL_Rect paddle, SDL_Rect ball) {
    return (ball.y >= paddle.y - ball.h) && (ball.y <= paddle.y + paddle.h) &&
           (((ball.x <= paddle.x + paddle.w) &&
             (ball.x + ball.w >= paddle.x)) ||
            ((paddle.x <= ball.x + ball.w) && (ball.x <= paddle.x)));
}

void move_ball() {
    if (is_colliding(display.ball, display.player_paddle) ||
        is_colliding(display.ball, display.computer_paddle)) {
        ball_stat.speed_x *= -1;
    }

    display.ball.x += ball_stat.speed_x;
    display.ball.y += ball_stat.speed_y;
}

void computer_move() {

}
