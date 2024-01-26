#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100

#define LINE_WIDTH 10
#define LINE_HEIGHT 40

#define SCORE_FONT_SIZE 72
#define SCORE_TO_WIN 1

#define GAP 15

#define BALL_SIDE 25
#define BALL_SPEED 15

#define PADDLE_SPEED_HUMAN 25
#define PADDLE_SPEED_COMPUTER 25

#define START_TEXT "Press any key to start"
#define END_TEXT "Game over. (R - reset)"

struct {
    SDL_Event event;
    bool j_pressed;
    bool k_pressed;
    bool up_pressed;
    bool down_pressed;
} input_status;

struct {
    int speed_x;
    int speed_y;
    int start_direction;
    bool first_reflect;
} ball_stat;

struct {
    int human;
    int computer;
    SDL_Surface *surface_human;
    SDL_Surface *surface_computer;
    SDL_Texture *texture_human;
    SDL_Texture *texture_computer;
    SDL_Rect rect_human;
    SDL_Rect rect_computer;
} score;

struct {
    bool started;
    bool human_won;
    bool computer_won;
} game_state;

struct {
    SDL_Rect message_rect;
    SDL_Texture *message_texture;
} text_message;

struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    TTF_Font *font;
    SDL_Rect player_paddle;
    SDL_Rect computer_paddle;
    SDL_Rect line;
    SDL_Rect ball;
    SDL_Color color;
} display;

void init_window();
void close_display();
void setup();
void render_game_frame();
void handle_input();
void move_ball();
void computer_move();
void human_move();
void check_goal();
void update_score();
void reset_ball();
void render_start_frame();
void detect_keys(SDL_Scancode scancode, bool pressed);
bool is_colliding(SDL_Rect paddle, SDL_Rect ball);
void create_text(char *message);

int main(void) {
    init_window();
    setup();
    create_text(START_TEXT);

    while (!game_state.started) {
        render_start_frame();
        handle_input();
    }

    create_text(END_TEXT);

    while (true) {
        if (!game_state.human_won && !game_state.computer_won) {
            handle_input();
            human_move();
            computer_move();
            move_ball();
            check_goal();
            render_game_frame();
        } else {
            handle_input();
            render_game_frame();
        }
    }
}

void detect_keys(SDL_Scancode scancode, bool pressed) {
    switch (scancode) {
    case SDL_SCANCODE_UP:
        input_status.up_pressed = pressed;
        break;

    case SDL_SCANCODE_DOWN:
        input_status.down_pressed = pressed;
        break;

    case SDL_SCANCODE_J:
        input_status.j_pressed = pressed;
        break;

    case SDL_SCANCODE_K:
        input_status.k_pressed = pressed;
        break;

    case SDL_SCANCODE_Q:
        close_display();
        exit(0);

    default:
        break;
    }
}

void handle_input() {
    SDL_Event event = input_status.event;

    while (SDL_PollEvent(&event)) {

        SDL_Scancode scancode = event.key.keysym.scancode;

        switch (event.type) {
        case SDL_QUIT:
            close_display();
            exit(0);

        case SDL_KEYDOWN:
            detect_keys(scancode, true);
            if (!game_state.started) {
                game_state.started = true;
                SDL_DestroyTexture(text_message.message_texture);
            }
            break;

        case SDL_KEYUP:
            detect_keys(scancode, false);
            break;
        }
    }
}

void render_game_frame() {
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
    for (int i = 0; i < WINDOW_HEIGHT; i += LINE_HEIGHT * 2) {
        display.line.y = i;
        SDL_RenderFillRect(display.renderer, &display.line);
    }

    SDL_RenderCopy(display.renderer, score.texture_human, NULL,
                   &score.rect_human);

    SDL_RenderCopy(display.renderer, score.texture_computer, NULL,
                   &score.rect_computer);

    if (game_state.human_won || game_state.computer_won) {
        SDL_RenderCopy(display.renderer, text_message.message_texture, NULL,
                       &text_message.message_rect);
    }

    SDL_RenderPresent(display.renderer);
}

void init_window() {

    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't intitalize SDL: %s", SDL_GetError());
        close_display();
        exit(1);
    }

    // Init TTF
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't intitalize TTF: %s", TTF_GetError());
        close_display();
        exit(1);
    }

    display.window =
        SDL_CreateWindow("Game of Pong", SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT,
                         SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    if (display.window == NULL) {
        printf("Could not create window: %s\n", SDL_GetError());
        close_display();
        exit(1);
    }

    display.renderer = SDL_CreateRenderer(display.window, -1,
                                          SDL_RENDERER_PRESENTVSYNC |
                                              SDL_RENDERER_ACCELERATED);

    display.texture = SDL_CreateTexture(
        display.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC,
        WINDOW_WIDTH, WINDOW_HEIGHT);

    // Init for text
    display.font = TTF_OpenFont("forward.ttf", SCORE_FONT_SIZE);

    if (display.font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load the font: %s",
                     TTF_GetError());
        close_display();
        exit(1);
    }

    SDL_Color white = {255, 255, 255};
    display.color = white;

    update_score();
    SDL_GetClipRect(score.surface_human, &score.rect_human);
    SDL_GetClipRect(score.surface_computer, &score.rect_computer);
}

void setup() {
    srandom(time(NULL));

    game_state.started = false;
    game_state.human_won = false;
    game_state.human_won = false;

    score.human = 0;
    score.computer = 0;

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

    score.rect_human.x = WINDOW_WIDTH - WINDOW_WIDTH / 5;
    score.rect_human.y = WINDOW_HEIGHT / 20;

    score.rect_computer.x = WINDOW_WIDTH / 5;
    score.rect_computer.y = WINDOW_HEIGHT / 20;

    display.ball.w = BALL_SIDE;
    display.ball.h = BALL_SIDE;

    ball_stat.start_direction = -1;

    reset_ball();
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

        if (ball_stat.first_reflect) {
            ball_stat.speed_x = BALL_SPEED * ball_stat.start_direction;
            ball_stat.first_reflect = false;
        }

        ball_stat.speed_x *= -1.05;
        ball_stat.speed_y += random() % 10;
    }

    else if (display.ball.y <= 0 ||
             display.ball.y + display.ball.h >= WINDOW_HEIGHT) {
        ball_stat.speed_y *= -1;
    }

    display.ball.x += ball_stat.speed_x;
    display.ball.y += ball_stat.speed_y;
}

void computer_move() {
    int ball_y = display.ball.y;
    int comp_y = display.computer_paddle.y;

    if (display.ball.x < WINDOW_WIDTH / 2) {
        if (ball_y + PADDLE_HEIGHT / 4 > comp_y &&
            ball_y - PADDLE_HEIGHT / 4 > comp_y &&
            ball_y + PADDLE_HEIGHT + GAP < WINDOW_HEIGHT) {
            display.computer_paddle.y += PADDLE_SPEED_COMPUTER / 1.5;
        }

        else if (ball_y + PADDLE_HEIGHT / 4 < comp_y &&
                 ball_y - PADDLE_HEIGHT / 4 < comp_y && ball_y > GAP) {
            display.computer_paddle.y -= PADDLE_SPEED_COMPUTER / 1.5;
        }
    }
}

void human_move() {
    if ((input_status.k_pressed || input_status.up_pressed) &&
        display.player_paddle.y > GAP) {
        display.player_paddle.y -= PADDLE_SPEED_HUMAN;
    }

    else if ((input_status.j_pressed || input_status.down_pressed) &&
             display.player_paddle.y + PADDLE_HEIGHT + GAP < WINDOW_HEIGHT) {
        display.player_paddle.y += PADDLE_SPEED_HUMAN;
    }
}

void check_goal() {
    if (display.ball.x <= 0) {
        score.human++;
        if (score.human == SCORE_TO_WIN) {
            game_state.human_won = true;
        } else {
            reset_ball();
        }
        update_score();
    }

    else if (display.ball.x + BALL_SIDE >= WINDOW_WIDTH) {
        score.computer++;
        if (score.computer == SCORE_TO_WIN) {
            game_state.computer_won = true;
        } else {
            reset_ball();
        }
        update_score();
    }
}

void reset_ball() {
    display.ball.x = WINDOW_WIDTH / 2 - BALL_SIDE / 2;
    display.ball.y = WINDOW_HEIGHT / 2;

    ball_stat.start_direction *= -1;
    ball_stat.first_reflect = true;

    ball_stat.speed_x = BALL_SPEED / 1.5 * ball_stat.start_direction;

    int sign = random() % 2 == 0 ? -1 : 1;
    ball_stat.speed_y = random() % 5 * sign;
}

void close_display() {
    TTF_CloseFont(display.font);
    SDL_DestroyTexture(display.texture);
    SDL_DestroyRenderer(display.renderer);
    SDL_DestroyWindow(display.window);
    SDL_DestroyTexture(score.texture_human);
    SDL_FreeSurface(score.surface_human);
    SDL_DestroyTexture(score.texture_computer);
    SDL_FreeSurface(score.surface_computer);
    SDL_DestroyTexture(text_message.message_texture);
    TTF_Quit();
    SDL_Quit();
}

void update_score() {
    char score_str[2];

    SDL_DestroyTexture(score.texture_human);
    SDL_DestroyTexture(score.texture_computer);

    SDL_FreeSurface(score.surface_human);
    SDL_FreeSurface(score.surface_computer);

    sprintf(score_str, "%d", score.human);

    score.surface_human =
        TTF_RenderText_Solid(display.font, score_str, display.color);
    score.texture_human =
        SDL_CreateTextureFromSurface(display.renderer, score.surface_human);

    sprintf(score_str, "%d", score.computer);

    score.surface_computer =
        TTF_RenderText_Solid(display.font, score_str, display.color);
    score.texture_computer =
        SDL_CreateTextureFromSurface(display.renderer, score.surface_computer);
}

void render_start_frame() {
    // Render background
    SDL_SetRenderDrawColor(display.renderer, 0, 0, 0, 0);
    SDL_RenderClear(display.renderer);

    SDL_RenderCopy(display.renderer, text_message.message_texture, NULL,
                   &text_message.message_rect);

    SDL_RenderPresent(display.renderer);
}

void create_text(char *message) {
    SDL_Surface *message_surface =
        TTF_RenderText_Solid(display.font, message, display.color);

    text_message.message_texture =
        SDL_CreateTextureFromSurface(display.renderer, message_surface);

    SDL_GetClipRect(message_surface, &text_message.message_rect);

    text_message.message_rect.x = WINDOW_WIDTH / 5;
    text_message.message_rect.y = WINDOW_HEIGHT / 3;

    SDL_FreeSurface(message_surface);
}
