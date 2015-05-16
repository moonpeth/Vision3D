#ifndef _CSURFACE_H_
    #define _CSURFACE_H_

#include <SDL.h>

class CSurface {
    public:
        CSurface();

    public:
//Load local image into memory.
        static SDL_Surface* OnLoad(char* File);

/* Draw image on the surface.
 * Putting Surf_Src on top of Surf_Dest.
 * The X, Y variables is the position on the Surf_Dest where we are drawing this surface to.
 */
        static bool OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y);
};

#endif
