#ifndef STATE_H
#define STATE_H

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

// Main game parameters
typedef struct {
    bool started;
    bool human_won;
    bool computer_won;
    int human_score;
    int computer_score;
} game_t;

// Ball parameters
typedef struct {
    int side;
    int x;
    int y;
    int speed_x;
    int speed_y;
    int start_direction;
    bool first_reflect;
} ball_t;

// Paddle parameters
typedef struct {
    int width;
    int height;
    int x;
    int y;
} paddle_t;

// Reset state for a new game
void reset_state(game_t *g);

// Set paddle parameters before the game
void create_paddle(paddle_t *p, int width, int height, int x, int y);

// Create a ball before the game
void create_ball(ball_t *b, int side);

// Return to center and reset speed
void reset_ball(ball_t *b, int side, int init_speed, int w_width, int w_height);
#endif // !STATE_H
