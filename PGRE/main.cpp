#include <QCoreApplication>

#include <SDL2/SDL.h>

#include "Core/core.h"
#include "Core/Math/vec.h"

using namespace PRGE;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Vec3<int> i = {1, 2, 4};
    Vec3<int> j = {3, 5, 5};
    Vec3<float> f;
    Vec3<float> e;

    f + e;

    j = i + j;

    int d = dot(i, j);

    cout << i << '\n' << j << '\n' << d << endl;


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
