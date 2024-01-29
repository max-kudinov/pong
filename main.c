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
#define SCORE_TO_WIN 10

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
    // game_t *game;
    // paddle_t *human_p;
    // paddle_t *computer_p;
    // ball_t *ball;

    // Input
    // key_status_t *key_status;

    // Render
    // score_t *score;
    text_t text;
    window_t window;
    // objects_t *objects;
    bg_color_t bg_c;

    bg_c.r = 0;
    bg_c.g = 0;
    bg_c.b = 0;
    bg_c.a = 0;

    init_window(&window, &text, FONT_NAME, FONT_SIZE, WINDOW_WIDTH, WINDOW_WIDTH,
                WINDOW_TITLE);

    // init_line(window, WINDOW_WIDTH / 2 - LINE_WIDTH / 2, LINE_HEIGHT,
    //           LINE_WIDTH);

    render_start_screen(&window, &text, bg_c);

    // init_window();
    // setup();
    // create_text(START_TEXT);
    //
    // while (!game_state.started) {
    //     render_start_frame();
    //     handle_input();
    // }
    //
    // create_text(END_TEXT);
    //
    // while (true) {
    //     if (!game_state.human_won && !game_state.computer_won) {
    //         handle_input();
    //         human_move();
    //         computer_move();
    //         move_ball();
    //         check_goal();
    //         render_game_frame();
    //     } else {
    //         handle_input();
    //         render_game_frame();
    //     }
    // }
}
