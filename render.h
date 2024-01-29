#ifndef RENDER_H
#define RENDER_H

#include "logic.h"
#include <SDL2/SDL_ttf.h>

typedef struct {
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect rect;
} score_t;

typedef struct {
    TTF_Font *font;
    SDL_Color color;
    SDL_Rect text_rect;
    SDL_Texture *text_texutre;
} text_t;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Rect line;
} window_t;

typedef struct {
    SDL_Rect human_paddle;
    SDL_Rect computer_paddle;
    SDL_Rect ball;
} objects_t;

typedef struct {
    int r;
    int g;
    int b;
    int a;
} bg_color_t;

// Init all SDL struct instances
void init_window(window_t *window, text_t *text, char *font, int font_size,
                 int window_h, int window_w, char *window_title);

// Free memory
void close_window(window_t *window, text_t *text);

// Init score with 0
void init_score(window_t *window, text_t *text, score_t *score, int x, int y);

// Update score with new values
void update_score(score_t *score, window_t *window, text_t *text, int value);

// Create text in a particular spot
void create_text(window_t *window, text_t *text, char *message, int x, int y);

// Render screen before the game
void render_start_screen(window_t *window, text_t *text, bg_color_t bg_c);

// Render main game
void render_game(window_t *window, objects_t *objects, bg_color_t bg_c,
                 int window_h);

// Render score
void render_score(window_t *window, text_t *text, score_t *s_human,
                  score_t *s_computer);

// Init line parameters
void init_line(window_t *window, int line_x, int line_h, int line_w);

// Init render objects with logic dimensions
void init_objects(objects_t *o, paddle_t *human_p, paddle_t *computer_p,
                  ball_t *b);

// Sync coordinates between game logic objects and render objects
void sync_coordinates(objects_t *o, paddle_t *human_p, paddle_t *computer_p,
                      ball_t *b);
#endif // !RENDER_H
