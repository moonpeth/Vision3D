#ifndef _AVATAR_H_
#define _AVATAR_H_
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "events.h"
#include "surface.h"
#include "sensor.h"

class CAvatar : public CEvent{
private:
bool should_be_running;
int window_width;
int window_height;
const char* window_title;

float world_rx;
float world_ry;
float camera_min_tz;
float camera_tx;
float camera_ty;
float camera_tz;
float world_origin_x;
float world_origin_y;
float world_origin_z;

float camera_aspect_ratio;
float camera_min_z;
float camera_max_z;
float camera_fovy;
bool needs_rendering;

GLuint texture[6];
GLuint textureColor;
char* images[6] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp","6.bmp"};
char* image = "1.bmp";
SDL_Surface* sdl_pimage;
SDL_Surface* Surf_Test;
//for mouse events
bool boutonR;
bool boutonL;
//position of light source
GLfloat light_position[4] = {300.0f, 300.0f, 3.0f, 1.0f};
//reflextion diffuse
GLfloat mat_diffuse[4]  = {1.0f, 1.0f, 1.0f, 1.0f};
//reflextion specular
GLfloat mat_specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};
//color of light
GLfloat mat_emission[4] = {1.5f, 0.5f, 0.5f, 1.0f};

CSensor sensor;


public:
    CAvatar();
    ~CAvatar();
    int OnExecute(int mode);
    bool OnInit(int mode);
    void OnCleanup();
    void OnLoop();
    void OnRender(int mode);

    void DrawDemo();
    void DrawSensor();

    void OnEvent(SDL_Event* Event);

    void InitSceneConstants();
    void SetPerspectiveProjectionMatrix();
    void SetOrthoProjectionMatrix();

    void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
    void OnResize(int w, int h);
    void OnExpose();
    void OnExit();
    void OnLButtonDown(int x, int y);
    void OnRButtonDown(int x, int y);
    void OnLButtonUp(int x, int y);
    void OnRButtonUp(int x, int y);
    void OnMouseMove(int x, int y, int xrel, int yrel);
    void OnMouseWheel(bool Up, bool Down);

};



#endif
