#include <QCoreApplication>

#include <SDL2/SDL.h>

#include "Core/core.h"
#include "Core/Math/vec.h"

using namespace PRGE;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Vec2<int> i = {1, 2};
    Vec2<int> j = {3, 5};

    j = i + j;

    SDL_Init(SDL_INIT_EVERYTHING);

    auto* window = SDL_CreateWindow("PRGE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);

    bool stay = true;
    SDL_Event event;

    while(stay) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    stay = false;
                break;

                case SDL_KEYUP:
                    if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                       stay = false;
                    }
                break;
            }
        }

        SDL_GL_SwapWindow(window);
    }

    SDL_DestroyWindow(window);

    return a.exec();
}
