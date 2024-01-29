#ifndef LOGIC_H
#define LOGIC_H

#include "input.h"
#include "render.h"
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

// Init render objects with logic dimensions
void init_objects(objects_t *objects, paddle_t *human_p, paddle_t *computer_p,
                  ball_t *ball);

// Sync coordinates between game logic objects and render objects
void sync_coordinates(objects_t *objects, paddle_t *human_p, paddle_t *computer_p,
                      ball_t *ball);

// Check for keyboard actions
void check_actions(game_t *game, window_t *window, text_t *text, key_status_t *key_status);
#endif // !LOGIC_H
