#include "input.h"

bool handle_input(key_status_t *key_status) {
    SDL_Event event;
    bool keydown = false;

    // Process every event in a queue
    while (SDL_PollEvent(&event)) {

        SDL_Scancode scancode = event.key.keysym.scancode;

        switch (event.type) {
        case SDL_QUIT:
            key_status->quit = true;
            break;

        case SDL_KEYDOWN:
            detect_keys(scancode, key_status, true);
            keydown = true;
            break;

        case SDL_KEYUP:
            detect_keys(scancode, key_status, false);
            break;
        }
    }

    return keydown;
}

void detect_keys(SDL_Scancode scancode, key_status_t *key_status,
                 bool pressed) {
    switch (scancode) {
    case SDL_SCANCODE_UP:
        key_status->up_pressed = pressed;
        break;

    case SDL_SCANCODE_DOWN:
        key_status->down_pressed = pressed;
        break;

    case SDL_SCANCODE_J:
        key_status->j_pressed = pressed;
        break;

    case SDL_SCANCODE_K:
        key_status->k_pressed = pressed;
        break;

    case SDL_SCANCODE_Q:
        key_status->quit = pressed;
        break;

    case SDL_SCANCODE_R:
        key_status->reload = pressed;
        break;

    default:
        break;
    }
}
