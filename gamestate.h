#ifndef GAMESTATE_H_
#define GAMESTATE_H_

#include "defs.h"
#include "common.h"

struct Gamestate
{

    SDL_Window* window;
    SDL_Renderer* renderer;

    SDL_Texture* background;
    SDL_Texture* menu_background;
    SDL_Texture* img_ball;
    SDL_Texture* img_paddle;
    SDL_Texture* win_screen;
    SDL_Texture* lose_screen;
    SDL_Texture* brick1;
    SDL_Texture* brick2;

    SDL_Rect paddle;
    SDL_Rect ball;
    SDL_Rect brick;

    bool bricks[ROW][COL];

    Gamestate();
    void init();
    bool show_menu();
    void show_end_screen(bool win);
    void update();
    void input();
    void render();
    void cleanup();
    void reset_bricks();

    SDL_Rect play_button;
    SDL_Rect exit_button;
    SDL_Rect try_again_button;
    SDL_Rect exit_button2;
    SDL_Rect live_left;

    TTF_Font* font;

    Mix_Chunk* sound_hit;
    Mix_Chunk* sound_menu;
    Mix_Music* sound_background;
    Mix_Music* win_sound;
    Mix_Music* lose_sound;

    bool running;
    bool wait_key_down;
    int live_count;

    double vX, vY;
    SDL_Color color;

    SDL_Texture* load_texture(const string& path);
    SDL_Texture* render_text(const string& text, SDL_Color color);
    void set_bricks(int row, int col);
    void load_map();
};


#endif // !GAMESTATE_H_
