#ifndef _GL_OBJECTS_H_
#define _GL_OBJECTS_H_
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>


void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length);
void DrawCube(float origin_x, float origin_y, float origin_z, float half_side, GLuint* texture);
//GLuint: unsigned long, GLsizei: long. arguments mean : weight,height,colorMode and source of texture.
GLuint Load2DTexture(GLsizei width,GLsizei height, int BytesPerPixel,const GLvoid * data);
void FillWindowWithTexture(GLuint texture_ID);
#endif
