#include "state.h"

void reset_state(game_t *game) {
    game->started = false;
    game->computer_won = false;
    game->human_won = false;

    game->human_score = 0;
    game->computer_score = 0;
}

void init_paddle(paddle_t *paddle, int width, int height, int x, int y) {
    paddle->width = width;
    paddle->height = height;
    paddle->x = x;
    paddle->y = y;
}

void init_ball(ball_t *ball, int side) {
    ball->side = side;
    ball->start_direction = -1;
}

void reset_ball(ball_t *ball, int side, int init_speed, int w_width,
                int w_height) {

    // Spawn in the center
    ball->x = w_width / 2 - side / 2;
    ball->y = w_height / 2;

    // Change start direction after every goal
    ball->start_direction *= -1;

    // Before first reflect speed should be slower
    ball->first_reflect = true;
    ball->speed_x = init_speed / 1.5 * ball->start_direction;

    // Randomize throwing angle
    int sign = random() % 2 == 0 ? -1 : 1;
    ball->speed_y = random() % 5 * sign;
}
