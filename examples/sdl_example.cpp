#include <BPX/BPX.hpp>
#include <SDL2/SDL.h>
#include <iostream>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to init SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window *win = SDL_CreateWindow(
        "BPX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600, SDL_WINDOW_SHOWN
    );

    if (win == nullptr) {
        std::cerr << "Failed to init window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    SDL_Surface* sdl_surface = SDL_GetWindowSurface(win);
    if (sdl_surface == nullptr) {
        std::cerr << "Failed to get window surface: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(win);
        SDL_Quit();
        return -1;
    }

    bpx::Image bpx_surface(
        sdl_surface->pixels, sdl_surface->w, sdl_surface->h,
        bpx::PixelFormat::BGRA_U8, false, false
    );

    bpx::Image image = bpx::Image(bpx::BLACK, 128, 128);
    bpx::circle_gradient(image, 64, 64, 64, bpx::GREEN, bpx::BLANK);

    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        bpx::fill(bpx_surface, bpx::BLUE);

        bpx::draw(
            bpx_surface, 0, 0, 800, 600,
            image, 0, 0, 128, 128,
            bpx::BlendMode::ADD
        );

        bpx::map(bpx_surface, [&bpx_surface](int x, int y, bpx::Color color){
            return blend(color, x ^ y, bpx::BlendMode::ADD);
        });

        bpx::line(bpx_surface, 0, 0, 800, 600, 4, bpx::RED);
        bpx::line(bpx_surface, 0, 600, 800, 0, 4, bpx::RED);

        SDL_UpdateWindowSurface(win);
    }

    return 0;
}
