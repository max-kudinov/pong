#include <stdlib.h>
#include <time.h>

#include "input.h"
#include "logic.h"
#include "render.h"
#include "state.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_TITLE "Pong"

#define PADDLE_WIDTH 20
#define PADDLE_HEIGHT 100

#define LINE_WIDTH 10
#define LINE_HEIGHT 40

#define FONT_NAME "forward.ttf"
#define FONT_SIZE 72
#define SCORE_TO_WIN 5

#define GAP 15

#define BALL_SIDE 25
#define BALL_SPEED 15

#define PADDLE_SPEED_HUMAN 25
#define PADDLE_SPEED_COMPUTER 25

#define START_TEXT "Press any key to start"
#define END_TEXT "Game over. (R - reset)"

int main(void) {

    // Random seed based on time
    srandom(time(NULL));

    // Game state
    game_t game = {0};
    paddle_t human_p = {0};
    paddle_t computer_p = {0};
    ball_t ball = {0};

    // Input
    key_status_t key_status = {0};

    // Render
    score_t human_s = {0};
    score_t computer_s = {0};
    text_t text = {0};
    window_t window = {0};
    objects_t objects = {0};
    bg_color_t bg_c = {0, 0, 0, 0};

    /*
     --------------------------------------------
     *              Initializing
     --------------------------------------------
     */

    reset_state(&game);

    // Human paddle setup
    init_paddle(&human_p,
            PADDLE_WIDTH,
            PADDLE_HEIGHT,
            WINDOW_WIDTH - 70,
            WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2);

    // Computer paddle setup
    init_paddle(&computer_p,
            PADDLE_WIDTH,
            PADDLE_HEIGHT, 70 - PADDLE_WIDTH,
            WINDOW_HEIGHT / 2 - PADDLE_HEIGHT / 2);

    // Ball setup
    init_ball(&ball, BALL_SIDE);

    // SDL graphics init
    init_window(&window,
            &text,
            FONT_NAME,
            FONT_SIZE,
            WINDOW_HEIGHT,
            WINDOW_WIDTH,
            WINDOW_TITLE);

    // Separator line parameters
    init_line(&window,
            WINDOW_WIDTH / 2 - LINE_WIDTH / 2,
            LINE_HEIGHT,
            LINE_WIDTH);

    // Score parameters for human
    init_score(&window,
            &text,
            &human_s,
            WINDOW_WIDTH - WINDOW_WIDTH / 5,
            WINDOW_HEIGHT / 20);

    // Score parameters for computer
    init_score(&window,
            &text,
            &computer_s,
            WINDOW_WIDTH / 5,
            WINDOW_HEIGHT / 20);

    // Pass dimensions from logic models to render sprites
    init_objects(&objects, &human_p, &computer_p, &ball);

    // Create text for start screen
    create_text(&window,
            &text,
            START_TEXT,
            WINDOW_WIDTH / 5,
            WINDOW_HEIGHT / 3);

    // Wait on start screen untill any input is given
    while (!handle_input(&key_status)) {
        check_actions(&game, &window, &text, &key_status);
        render_static_screen(&window, &text, bg_c);
    }

    // Move ball to the center
    reset_ball(&ball,
            BALL_SIDE,
            BALL_SPEED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT);

    // Create text for the end
    create_text(&window,
            &text,
            END_TEXT,
            WINDOW_WIDTH / 5,
            WINDOW_HEIGHT / 3);

    // Continue intill player quits
    while (true) {

        // Update score values
        update_score(&human_s, &window, &text, game.human_score);
        update_score(&computer_s, &window, &text, game.computer_score);

        // If the game is in progress
        if (!game.human_won && !game.computer_won) {

            handle_input(&key_status);

            // Sync between logic models and render sprites
            sync_coordinates(&objects, &human_p, &computer_p, &ball);

            // Change player paddle coordinates
            human_move(&key_status,
                    &human_p,
                    PADDLE_SPEED_HUMAN,
                    GAP,
                    WINDOW_HEIGHT);

            // Change computer paddle coordinates
            computer_move(&computer_p,
                    &ball,
                    PADDLE_SPEED_COMPUTER,
                    GAP,
                    WINDOW_HEIGHT,
                    WINDOW_WIDTH);

            // Change ball coordinates
            move_ball(&ball,
                    &human_p,
                    &computer_p,
                    BALL_SPEED,
                    WINDOW_HEIGHT);

            if (check_goal(&game, &ball, SCORE_TO_WIN, WINDOW_WIDTH)) {
                
                // Update score values for both human and computer
                update_score(&human_s, &window, &text, game.human_score);
                update_score(&computer_s, &window, &text, game.computer_score);

                // Move the ball to the center again
                reset_ball(&ball,
                        BALL_SIDE,
                        BALL_SPEED,
                        WINDOW_WIDTH,
                        WINDOW_HEIGHT);
            }

            // Main game scene
            render_game(&window, &objects, bg_c, WINDOW_HEIGHT);

            // Score on top
            render_score(&window, &text, &human_s, &computer_s);

            // Check for quit signal
            check_actions(&game, &window, &text, &key_status);

            // Draw new frame on the screen
            SDL_RenderPresent(window.renderer);

        } else {

            handle_input(&key_status);

            // Check for quit signal
            check_actions(&game, &window, &text, &key_status);

            // Render game
            render_game(&window, &objects, bg_c, WINDOW_HEIGHT);
            render_score(&window, &text, &human_s, &computer_s);

            // Render text in the center
            SDL_RenderCopy(window.renderer, text.text_texture, NULL,
                           &text.text_rect);

            // Draw frame
            SDL_RenderPresent(window.renderer);
        }
    }

    // Free memory and quit
    close_window(&window, &text);
}
