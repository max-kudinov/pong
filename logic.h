#ifndef LOGIC_H
#define LOGIC_H

#include "input.h"
#include "state.h"

// Update human paddle coordinates
void human_move(key_status_t *key_status, paddle_t *paddle, int paddle_speed,
                int gap, int w_height);

// Update computer paddle coordinates
void computer_move(paddle_t *paddle, ball_t *ball, int paddle_speed, int gap,
                   int widow_h, int window_w);

// Update ball coordinates
void move_ball(ball_t *ball, paddle_t *human_p, paddle_t *computer_p,
               int ball_speed, int window_h);

// Check for ball and paddle collisions
bool is_colliding(paddle_t *paddle, ball_t *ball);

// Returns true if there's a goal, otherwise false
bool check_goal(game_t *game, ball_t *ball, int win_score, int window_w);

#endif // !LOGIC_H
