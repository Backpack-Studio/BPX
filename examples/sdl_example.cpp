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

    bpx::Image bpx_surface = {
        sdl_surface->pixels,
        sdl_surface->w, sdl_surface->h,
        bpx::PixelFormat::BGRA_U8, false
    };

    // Generate xor pattern
    bpx::Image im_xor = bpx::Image(1024, 1024, bpx::BLUE);
    bpx::map(im_xor, [&bpx_surface](int x, int y, bpx::Color color){
        return blend(color, x ^ y, bpx::BlendMode::ADD);
    });

    // Generate grid pattern
    bpx::Image im_grid = bpx::generate_grid(1024, 1024, 16);

    // Generate linear gradient
    bpx::ColorRamp ramp1(bpx::WHITE, bpx::BLACK);
    bpx::Image im_linear = bpx::generate_gradient_linear(1024, 1024, ramp1, 512, 0, 512, 1024);

    // Generate radial gradient
    bpx::ColorRamp ramp2({
        { bpx::WHITE, 0.0f },
        { bpx::RED, 0.2f },
        { bpx::GREEN, 0.4f },
        { bpx::BLUE, 0.6f },
        { bpx::RED, 0.8f },
        { bpx::BLACK, 1.0f }
    });
    bpx::Image im_radial = bpx::generate_gradient_radial(1024, 1024, ramp2, 512, 512, 1024, 512);

    // Main program

    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
                break;
            }
        }

        bpx::fill(bpx_surface, bpx::BLACK);

        bpx::draw(bpx_surface, 0, 0, 400, 300, im_xor);
        bpx::draw(bpx_surface, 400, 0, 400, 300, im_grid);
        bpx::draw(bpx_surface, 0, 300, 400, 300, im_linear);
        bpx::draw(bpx_surface, 400, 300, 400, 300, im_radial);

        SDL_UpdateWindowSurface(win);
    }

    return 0;
}
