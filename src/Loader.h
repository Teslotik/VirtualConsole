#ifndef LOADER_H
#define LOADER_H

#include <iostream>
#include <fstream>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "draw/Atlas.h"
#include "draw/Canvas.h"
#include "draw/Tilemap.h"

namespace HVC {


struct Loader {
    template<class T>
    T *surface(const char* url) {
        SDL_Surface *texture = IMG_Load(url);
        SDL_Surface *normalized = SDL_ConvertSurface(texture, SDL_PIXELFORMAT_ARGB8888);;

        T *surface = new T(normalized->w, normalized->h);
        SDL_LockSurface(normalized);
        surface->load(normalized->pixels);
        SDL_UnlockSurface(normalized);

        SDL_DestroySurface(normalized);
        SDL_DestroySurface(texture);
        
        std::cout << "[Info] Surface loaded: " << url << std::endl;
        return surface;
    }

    inline string read(const char *url) {
        ifstream stream;
        stringstream output;
        try {
            stream.open(url);
            output << stream.rdbuf();
            stream.close();
        } catch (ifstream::failure) {
            cout << "[EXCEPTION] Failed to read file: " << url << endl;
            return nullptr;
        }
        return output.str();
    }

    inline void write(const char *url, const char *text) {
        ofstream stream;
        try {
            stream.open(url);
            stream << text;
            stream.close();
        } catch (ofstream::failure) {
            cout << "[EXCEPTION] Failed to write file: " << url << endl;
        }
    }
};



} // namespace HVC

#endif