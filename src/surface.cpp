#include "surface.h"

CSurface::CSurface() {
}

SDL_Surface* CSurface::OnLoad(char* File) {
    SDL_Surface* Surf_Temp = NULL;
    SDL_Surface* Surf_Return = NULL;
    //SDL_LoadBMP takes the file we want to load as the argument, and returns a surface.
    if((Surf_Temp = SDL_LoadBMP(File)) == NULL) {
        return NULL;
    }
    //SDL_DisplayFormat takes a surface already loaded, and returns a new surface using the same format as the display.
    Surf_Return = SDL_DisplayFormat(Surf_Temp);
    //We have to free the old surface since it creates a new one.
    SDL_FreeSurface(Surf_Temp);

    return Surf_Return;
}

bool CSurface::OnDraw(SDL_Surface* Surf_Dest, SDL_Surface* Surf_Src, int X, int Y) {
    //Make sure we have valid surfaces.
    if(Surf_Dest == NULL || Surf_Src == NULL) {
        return false;
    }
    //Creates the dimensions for a rectangle.
    SDL_Rect DestR;
    //We care only where to draw but not the size. So we just assign X, Y coordinates to the destination.
    DestR.x = X;
    DestR.y = Y;

    SDL_BlitSurface(Surf_Src, NULL, Surf_Dest, &DestR);

    return true;
}
