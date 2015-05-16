#include "gl_objects.h"
#include <iostream>
void DrawFrame(float origin_x, float origin_y, float origin_z, float frame_length){
  glBegin(GL_LINES);
  //draw a horizontal red line, composed by two points
    glColor3f(1,0,0);
    //start point
    glVertex3f(origin_x,origin_y,origin_z);
    //end point
    glVertex3f(origin_x+frame_length,origin_y,origin_z);
 //draw a vertical blue line
    glColor3f(0,0,1);
    glVertex3f(origin_x,origin_y,origin_z);
    glVertex3f(origin_x,origin_y+frame_length,origin_z);
 //green depth line
    glColor3f(0,1,0);
    glVertex3f(origin_x,origin_y,origin_z);
    glVertex3f(origin_x,origin_y,origin_z+frame_length);
    std::cout<< "\nfaut afficher le frame mtn"<< std::endl;
  glEnd();
}
void DrawCube(float origin_x, float origin_y, float origin_z, float
half_side, GLuint* texture)
{
        // front

     glBindTexture(GL_TEXTURE_2D, texture[0]);
     std::cout<< "texture[0]"<< std::endl;
      std::cout<< texture[0]<< std::endl;
        std::cout<< texture[1]<< std::endl;
        std::cout<< texture[4]<< std::endl;
    glColor3f(1,1,1);
    glBegin(GL_QUADS);
      //  glColor3f(0, 1, 0);
        // glEnable(GL_TEXTURE_2D);
         glTexCoord2f(1, 1);glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
        glTexCoord2f(1, 0); glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
        glTexCoord2f(0, 0); glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z+half_side);
        glTexCoord2f(0, 1); glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z+half_side);
        // glDisable(GL_TEXTURE_2D);
         glEnd();

        // back
           glBindTexture(GL_TEXTURE_2D, texture[1]);
           glColor3f(1,1,1);
           std::cout<< texture[1]<< std::endl;
           glBegin(GL_QUADS);
        //glColor3f(0, 0.5, 0);
          glTexCoord2f(0, 0); glVertex3f(origin_x+half_side, origin_y+half_side,
    origin_z-half_side);
           glTexCoord2f(0, 1);  glVertex3f(origin_x+half_side, origin_y-half_side,
    origin_z-half_side);
           glTexCoord2f(1, 1);  glVertex3f(origin_x-half_side, origin_y-half_side,
    origin_z-half_side);
            glTexCoord2f(1, 0); glVertex3f(origin_x-half_side, origin_y+half_side,
    origin_z-half_side);
            glEnd();

        // left
        glBindTexture(GL_TEXTURE_2D, texture[2]);
        glColor3f(1, 1, 1);
        glBegin(GL_QUADS);
        //glColor3f(0.5, 0, 0);
       glTexCoord2f(1, 1);glVertex3f(origin_x-half_side, origin_y-half_side, origin_z-half_side);
       glTexCoord2f(1, 0); glVertex3f(origin_x-half_side, origin_y+half_side, origin_z-half_side);
       glTexCoord2f(0, 0); glVertex3f(origin_x-half_side, origin_y+half_side, origin_z+half_side);
       glTexCoord2f(0, 1); glVertex3f(origin_x-half_side, origin_y-half_side, origin_z+half_side);
         glEnd();

        //right
          glBindTexture(GL_TEXTURE_2D, texture[4]);
            glColor3f(1,1,1);
        glBegin(GL_QUADS);
        //glColor3f(1, 0, 0);
        glTexCoord2f(1, 0); glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z-half_side);
        glTexCoord2f(0, 0); glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
         glTexCoord2f(0, 1);glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
        glTexCoord2f(1, 1); glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z-half_side);
         glEnd();
        // top
          glBindTexture(GL_TEXTURE_2D, texture[3]);
            glColor3f(1,1,1);
        glBegin(GL_QUADS);
        //glColor3f(0, 0, 1);
        glTexCoord2f(1, 0); glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z+half_side);
        glTexCoord2f(0, 0); glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z+half_side);
         glTexCoord2f(0, 1);glVertex3f(origin_x+half_side, origin_y+half_side,
origin_z-half_side);
         glTexCoord2f(1, 1);glVertex3f(origin_x-half_side, origin_y+half_side,
origin_z-half_side);
        glEnd();
        // bottom
         glBindTexture(GL_TEXTURE_2D, texture[5]);
        glColor3f(1, 1, 1);

        glBegin(GL_QUADS);
        glTexCoord2f(1, 1); glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z+half_side);
         glTexCoord2f(0, 1);glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z+half_side);
        glTexCoord2f(0, 0); glVertex3f(origin_x-half_side, origin_y-half_side,
origin_z-half_side);
        glTexCoord2f(1, 0); glVertex3f(origin_x+half_side, origin_y-half_side,
origin_z-half_side);
        std::cout<< "\nfaut afficher le cube mtn"<< std::endl;
        glEnd();
}

GLuint Load2DTexture(GLsizei width,GLsizei height, int BytesPerPixel,const GLvoid * data){
 GLuint texture_ID;
 glGenTextures(1,&texture_ID);
 glBindTexture(GL_TEXTURE_2D,texture_ID);

 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 int color_mode = (BytesPerPixel == 3 ? GL_RGB : GL_RGBA);
 glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, data);

 return texture_ID;
}

//pour le video
void FillWindowWithTexture(GLuint texture_ID){
    //cette, on utilise le texture viens de la caméra
    glBindTexture(GL_TEXTURE_2D, texture_ID);
    glColor3f(1,1,1);
    glBegin(GL_QUADS);

   glTexCoord2f(0, 1); glVertex3f(0, 0,
0);
    glTexCoord2f(0, 0);  glVertex3f(0, 1,
0);
    glTexCoord2f(1, 0);  glVertex3f(1, 1,
0);
     glTexCoord2f(1, 1); glVertex3f(1, 0,
0);

     glEnd();
}
