#include "logic.h"

void human_move(key_status_t *key_status, paddle_t *p, int paddle_speed,
                int gap, int window_h) {

    // If key down is pressed and there's space, then go down
    if ((key_status->k_pressed || key_status->up_pressed) && p->y > gap)
        p->y -= paddle_speed;

    // If key up is pressed and there's space, go up
    else if ((key_status->j_pressed || key_status->down_pressed) &&
             p->y + p->height + gap < window_h)
        p->y += paddle_speed;
}

void computer_move(paddle_t *paddle, ball_t *ball, int paddle_speed, int gap,
                   int widow_h, int window_w) {

    // Move only if the ball is on computer's side of the window
    if (ball->x < window_w / 2) {

        // If the ball is more than 1/4 of a paddle above
        if ((ball->y + paddle->height / 4 > paddle->y) &&
            (ball->y - paddle->height / 4 > paddle->y) &&
            (ball->y + paddle->height + gap < widow_h))

            // Move up
            paddle->y += paddle_speed;

        // If more than 1/4 down
        else if ((ball->y + paddle->height / 4 < paddle->y) &&
                 (ball->y - paddle->height / 4 < paddle->y) && (ball->y > gap))

            // Move down
            paddle->y -= paddle_speed;
    }
}

void move_ball(ball_t *ball, paddle_t *human_p, paddle_t *computer_p,
               int ball_speed, int window_h) {

    // If there's a collision
    if (is_colliding(computer_p, ball) || is_colliding(human_p, ball)) {

        // If it's the first, then increse the initial speed to normal
        if (ball->first_reflect) {
            ball->speed_x = ball_speed * ball->start_direction;
            ball->first_reflect = false;
        }

        // Reflect x speed and slightly increase
        ball->speed_x *= -1.05;
        // Increase y speed on a random value
        ball->speed_y += random() % 10;
    }

    // If there's a collision with top or bottom
    else if (ball->y <= 0 || ball->y + ball->side >= window_h) {
        // Then reflect y speed
        ball->speed_y *= -1;
    }

    // Change coordinates by speed values
    ball->x += ball->speed_x;
    ball->y += ball->speed_y;
}

bool is_colliding(paddle_t *paddle, ball_t *ball) {
    // If ball y within paddle y and ball x >= or <= paddle x
    return (ball->y >= paddle->y - ball->side) &&
           (ball->y <= paddle->y + paddle->height) &&
           (((ball->x <= paddle->x + paddle->width) &&
             (ball->x + ball->side >= paddle->x)) ||
            ((paddle->x <= ball->x + ball->side) && (ball->x <= paddle->x)));
}

bool check_goal(game_t *game, ball_t *ball, int win_score, int window_w) {
    bool goal = false;

    // If ball has touched the left edge
    if (ball->x <= 0) {
        game->human_score++;

        if (game->human_score == win_score)
            game->human_won = true;

        goal = true;
    }

    // If ball has touched the right edge
    else if (ball->x + ball->side >= window_w) {
        game->computer_score++;
        if (game->computer_score == win_score)
            game->computer_won = true;

        goal = true;
    }

    return goal;
}

void check_actions(game_t *game, window_t *window, text_t *text, key_status_t *key_status) {
    if (key_status->quit) {
        close_window(window, text);
        exit(0);
    }
    else if (key_status->reload) {
        reset_state(game);
    }
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
