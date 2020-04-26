#include <SDL2/SDL.h>
#include <stdio.h>

/* read about mandelbrot set here: https://en.wikipedia.org/wiki/Mandelbrot_set */

const int MANDELBROT_CHECK_ITERATIONS = 40;

int mandelbrot_scale(double real_component, double imaginary_component) {
    double z_a = real_component;
    double z_b = imaginary_component;

    int i;
    for (i = 0; i < MANDELBROT_CHECK_ITERATIONS; ++i) {
        double tmp_z_a = z_a;
        z_a = z_a * z_a - z_b * z_b + real_component;
        z_b = 2 * tmp_z_a * z_b + imaginary_component;
        if (abs(z_a + z_b) >= 2)
            break;
    }
    return i;
}

double scale(double num, double src_min, double src_max, double dest_min, double dest_max) {
    return ((num - src_min) * (dest_max - dest_min) + (dest_min * src_max) -
            (dest_min * src_min)) / (src_max - src_min);
}

void draw_mandelbrot(SDL_Renderer* renderer, int width, int height, double x_mov, double y_mov, double zoom) {
    for (int a = 0; a < width; ++a) {
        for (int b = 0; b < height; ++b) {
            double imaginary_component = scale(b, 0, height - 1, -2/zoom, 2/zoom);
            double real_component = scale(a, 0, width - 1, -2/zoom, 2/zoom);
            imaginary_component += y_mov;
            real_component += x_mov;
            double color_scale = scale(mandelbrot_scale(real_component, imaginary_component), 0,
                                       MANDELBROT_CHECK_ITERATIONS - 1, 0, 255);
            SDL_SetRenderDrawColor(renderer, color_scale, color_scale, color_scale, 0x00);
            SDL_RenderDrawPoint(renderer, a, b);
        }
    }
}

int main(int argc, char *argv[])
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Event event;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
        return 3;
    }

    if (SDL_CreateWindowAndRenderer(320, 240, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't create window and renderer: %s", SDL_GetError());
        return 3;
    }

    SDL_PollEvent(&event);


    /* control variables */
    double zoom = 1;
    double y_mov = 0;
    double x_mov = -0.1;

    /* stop the loop */
    int quit = 0;
    int zoom_cnt = 0;
    double zoom_value = 0.1;

    /* used for zooming in because variables cant be declared in case statements */
    double to_zoom = zoom_value;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                    {
                    case SDLK_LEFT:
                    case SDLK_h:
                        x_mov -= 0.001;
                        printf("x_mov: %f\n", x_mov);
                        break;
                    case SDLK_RIGHT:
                    case SDLK_l:
                        x_mov += 0.001;
                        printf("x_mov: %f\n", x_mov);
                        break;
                    case SDLK_UP:
                    case SDLK_k:
                        y_mov -= 0.001;
                        printf("y_mov: %f\n", y_mov);
                        break;
                    case SDLK_DOWN:
                    case SDLK_j:
                        y_mov += 0.001;
                        printf("y_mov: %f\n", y_mov);
                        break;
                    case SDLK_i:
                        zoom *= 2;
                        printf("zoom: %f\n", zoom);
                        break;
                    case SDLK_o:
                        zoom /= 2;
                        printf("zoom: %f\n", zoom);
                        break;
                    case SDLK_w:
                        y_mov -= 0.01;
                        printf("y_mov: %f\n", y_mov);
                        break;
                    case SDLK_s:
                        y_mov += 0.01;
                        printf("y_mov: %f\n", y_mov);
                        break;
                    case SDLK_d:
                        x_mov += 0.01;
                        printf("x_mov: %f\n", x_mov);
                        break;
                    case SDLK_a:
                        x_mov -= 0.01;
                        printf("x_mov: %f\n", x_mov);
                        break;
                    }
                break;
            case SDL_QUIT:
                quit = 1;
                break;
            default:
                break;
            }
            fflush(stdout);
        }
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xff, 0x00);
        SDL_RenderClear(renderer);
        int width;
        int height;
        SDL_GetWindowSize(window, &width, &height);
        draw_mandelbrot(renderer, width, height, x_mov, y_mov, zoom);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
