#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include <stdbool.h>

typedef struct {
    bool j_pressed;
    bool k_pressed;
    bool up_pressed;
    bool down_pressed;
    bool quit;
    bool reload;
} key_status_t;

/* Handle keyboard input events
returns true if any key was down, otherwise false */
bool handle_input(key_status_t *key_status);

// Set flags for pressed keys or actions
void detect_keys(SDL_Scancode scancode, key_status_t *key_status, bool pressed);

#endif // !INPUT_H
