#include "render.h"

void init_window(window_t *window, text_t *text, char *font, int font_size,
                 int window_h, int window_w, char *window_title) {

    // Init SDL2
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't intitalize SDL: %s", SDL_GetError());
        close_window(window, text);
        exit(1);
    }

    // Init TTF
    if (TTF_Init() == -1) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "Couldn't intitalize TTF: %s", TTF_GetError());
        close_window(window, text);
        exit(1);
    }

    // Create a window intance
    window->window = SDL_CreateWindow(
        window_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        window_w, window_h, SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

    if (window->window == NULL) {
        printf("Could not create a window: %s\n", SDL_GetError());
        close_window(window, text);
        exit(1);
    }

    // Create a renderer intance
    window->renderer = SDL_CreateRenderer(window->window, -1,
                                          SDL_RENDERER_PRESENTVSYNC |
                                              SDL_RENDERER_ACCELERATED);

    if (window->renderer == NULL) {
        printf("Could not create a renderer: %s\n", SDL_GetError());
        close_window(window, text);
        exit(1);
    }

    // Create a texture instance
    window->texture =
        SDL_CreateTexture(window->renderer, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STATIC, window_w, window_h);

    if (window->texture == NULL) {
        printf("Could not create a texture: %s\n", SDL_GetError());
        close_window(window, text);
        exit(1);
    }

    // Set font and size
    text->font = TTF_OpenFont(font, font_size);

    if (text->font == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't load the font: %s",
                     TTF_GetError());
        close_window(window, text);
        exit(1);
    }

    // Set text color
    SDL_Color white = {255, 255, 255};
    text->color = white;

    SDL_SetRenderTarget(window->renderer, window->texture);
}

void close_window(window_t *window, text_t *text) {

    // Text
    TTF_CloseFont(text->font);
    SDL_DestroyTexture(text->text_texutre);

    // Window
    SDL_DestroyTexture(window->texture);
    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);

    TTF_Quit();
    SDL_Quit();
}

void init_score(window_t *window, text_t *text, score_t *score, int x, int y) {

    // Create new surface and texture with 0 value
    score->surface = TTF_RenderText_Solid(text->font, "0", text->color);
    score->texture =
        SDL_CreateTextureFromSurface(window->renderer, score->surface);

    // Clip rect size
    SDL_GetClipRect(score->surface, &score->rect);

    // Set score coordinates
    score->rect.x = x;
    score->rect.y = y;
}

void update_score(score_t *score, window_t *window, text_t *text, int value) {
    char *score_str = NULL;

    // Free memory from previous score
    SDL_DestroyTexture(score->texture);
    SDL_FreeSurface(score->surface);

    // Convert score value to string
    sprintf(score_str, "%d", value);

    // Create surface and texture
    score->surface = TTF_RenderText_Solid(text->font, score_str, text->color);
    score->texture =
        SDL_CreateTextureFromSurface(window->renderer, score->surface);
}

void create_text(window_t *window, text_t *text, char *message, int x, int y) {

    // Free previous text
    SDL_DestroyTexture(text->text_texutre);

    // Create new text surface and texture
    SDL_Surface *text_surface =
        TTF_RenderText_Solid(text->font, message, text->color);

    text->text_texutre =
        SDL_CreateTextureFromSurface(window->renderer, text_surface);

    // Clip text size
    SDL_GetClipRect(text_surface, &text->text_rect);

    // Set rect coordinates
    text->text_rect.x = x;
    text->text_rect.y = y;

    // Free text surface
    SDL_FreeSurface(text_surface);
}

void render_start_screen(window_t *window, text_t *text, bg_color_t bg_c) {

    // Render background
    SDL_SetRenderDrawColor(window->renderer, bg_c.r, bg_c.g, bg_c.b,
                           bg_c.a);
    SDL_RenderClear(window->renderer);

    // Render text
    SDL_RenderCopy(window->renderer, text->text_texutre, NULL,
                   &text->text_rect);

    SDL_RenderPresent(window->renderer);
}

void render_game(window_t *window, objects_t *objects, bg_color_t bg_c,
                 int window_h) {

    // Render background
    SDL_SetRenderDrawColor(window->renderer, bg_c.r, bg_c.g, bg_c.b,
                           bg_c.a);
    SDL_RenderClear(window->renderer);

    // Render paddles
    SDL_SetRenderDrawColor(window->renderer, 255, 255, 255, 0);
    SDL_RenderFillRect(window->renderer, &objects->human_paddle);
    SDL_RenderFillRect(window->renderer, &objects->computer_paddle);

    // Render ball
    SDL_RenderFillRect(window->renderer, &objects->ball);

    // Render center line
    for (int i = 0; i < window_h; i += window->line.h * 2) {
        window->line.y = i;
        SDL_RenderFillRect(window->renderer, &window->line);
    }
}

void render_score(window_t *window, text_t *text, score_t *s_human,
                  score_t *s_computer) {

    // Render human score
    SDL_RenderCopy(window->renderer, s_human->texture, NULL, &s_human->rect);

    // Render computer score
    SDL_RenderCopy(window->renderer, s_computer->texture, NULL,
                   &s_computer->rect);
}

void init_line(window_t *window, int line_x, int line_h, int line_w) {
    window->line.x = line_x;
    window->line.h = line_h;
    window->line.w = line_w;
}

void init_objects(objects_t *objects, paddle_t *human_p, paddle_t *computer_p,
                  ball_t *ball) {

    // Set dimensions for human paddle
    objects->human_paddle.w = human_p->width;
    objects->human_paddle.h = human_p->height;

    // Set dimensions for computer paddle
    objects->computer_paddle.w = computer_p->width;
    objects->computer_paddle.h = computer_p->height;

    // Set size for ball
    objects->ball.w = ball->side;
    objects->ball.h = ball->side;
}

void sync_coordinates(objects_t *objects, paddle_t *human_p,
                      paddle_t *computer_p, ball_t *ball) {

    // Sync human
    objects->human_paddle.x = human_p->x;
    objects->human_paddle.y = human_p->y;

    // Sync computer
    objects->computer_paddle.x = computer_p->x;
    objects->computer_paddle.y = computer_p->y;

    // Sync ball
    objects->ball.x = ball->x;
    objects->ball.y = ball->y;
}
