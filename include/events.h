#ifndef _EVENTS_H_
#define _EVENTS_H_

#include <SDL.h>

class CEvent{
public:
    CEvent();
    virtual ~CEvent();

    virtual void OnEvent(SDL_Event* Event);
    //for the key events
    virtual void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    virtual void OnResize(int w, int h);
    virtual void OnExpose();
    virtual void OnExit();
    //for the mouse events
    virtual void OnLButtonDown(int x, int y);
    virtual void OnRButtonDown(int x, int y);
    virtual void OnLButtonUp(int x, int y);
    virtual void OnRButtonUp(int x, int y);
    virtual void OnMouseMove(int x, int y, int xrel, int yrel);
    virtual void OnMouseWheel(bool Up, bool Down);
};

#endif
