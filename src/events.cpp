#include "events.h"

CEvent::CEvent(){}
CEvent::~CEvent(){}
void CEvent::OnEvent(SDL_Event* Event){}
void CEvent::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){}
void CEvent::OnResize(int w, int h){}
void CEvent::OnExpose(){}
void CEvent::OnExit(){}
void CEvent::CEvent::OnLButtonDown(int x, int y){}
void CEvent::OnRButtonDown(int x, int y){}
void CEvent::OnLButtonUp(int x, int y){}
void CEvent::OnRButtonUp(int x, int y){}
void CEvent::OnMouseMove(int x, int y, int xrel, int yrel){}
void CEvent::OnMouseWheel(bool Up, bool Down){}


