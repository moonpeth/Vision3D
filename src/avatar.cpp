#define SDL_VIDEO_MODE_OPTIONS (SDL_RESIZABLE |SDL_OPENGL)
#define SDL_DEPTH 32
#define SCENE_ROTATION_STEP 5
#define CAMERA_Z_OFFSET 2
#define CAMERA_TRANSLATION_STEP 0.1
#define RDR_FRAME_LENGTH   1
#define RDR_CUBE_HALF_SIDE 0.5

#define MODE_DEMO 1
#define MODE_SENSOR 0

#include "avatar.h"
#include "gl_objects.h"
#include <iostream>


CAvatar::CAvatar(){
    should_be_running = true;
    window_width=0;
    window_height=0;
    window_title=NULL;
    sdl_pimage=NULL;
    Surf_Test = NULL;
    world_rx = 0;
    world_ry = 0;
    camera_min_tz = 0;
    camera_tx = 0;
    camera_ty = 0;
    camera_tz = 0;
    world_origin_x = 0;
    world_origin_y = 0;
    world_origin_z = 0;
    camera_aspect_ratio =0;
    camera_min_z = 0;
    camera_max_z = 0;
    camera_fovy = 0;
    needs_rendering = true;
    boutonR = false;
    boutonL = false;
    InitSceneConstants();

}

CAvatar::~CAvatar(){}
/*
* Entrance fonction
*/
int CAvatar::OnExecute(int mode){
//Inisialize the window
    if(OnInit(mode) == false){
        return -1;
    }

    SDL_Event event;
    SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
    should_be_running = true;
        //handle the events.
    while(should_be_running){
        //get events from the queue FIFO, and handle it .
        while(SDL_PollEvent(&event)){
            OnEvent(&event);
        }
        OnLoop();
        //draw objets when on the mode demo
        OnRender(mode);
    }
    return 0;
}

/*
* Inisialize the window
*/
bool CAvatar::OnInit(int mode){

    //LA FENETRE:
    char sdl_wdw_pos[] = "SDL_VIDEO_WINDOW_POS";
    char sdl_wdw_ctr[] = "SDL_VIDEO_CENTERED=1";
    putenv(sdl_wdw_pos);
    putenv(sdl_wdw_ctr);

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){ return false;}

    window_width = 640;
    window_height = 480;
    window_title = "Avatar Main Window LOL";
    SDL_WM_SetCaption(window_title,0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);

    sdl_pimage = SDL_SetVideoMode(window_width,window_height,SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);
    if(sdl_pimage == NULL) return false;
     glEnable(GL_TEXTURE_2D);
     glClearColor(0,0,0,0);
     glViewport(0,0,window_width,window_height);
     camera_aspect_ratio = ((float)window_width)/((float)window_height);
     camera_min_z = 0.1;
     camera_max_z = 10;
     camera_fovy = 60;
//ON MODE DEMO
    if (mode == MODE_DEMO){
        //Load new surface images,texture

        for(int i=0; i<6;i++){

            if((Surf_Test = CSurface::OnLoad((char*)images[i])) == NULL) {
                  return false;
              }
              texture[i] = Load2DTexture(Surf_Test->w,Surf_Test->h,SDL_DEPTH,Surf_Test->pixels);
              std::cout<< texture[i]<< std::endl;
        }

        //pour lumière
        //utilise les lumière
        glEnable(GL_LIGHTING);
        //utilise l'effet de la transition
        glShadeModel(GL_SMOOTH);
        //source de lumière 0 (ya 0-7 à choisir)
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0,GL_POSITION,light_position);
        //for the light material
        //glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);  //colorBronzeDiff
        glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);  //colorBronzeSpec
        glMaterialf(GL_FRONT, GL_SHININESS, 180.0);
        glMaterialfv(GL_FRONT,GL_EMISSION, mat_emission); //mat_emission
        //  glDisable(GL_LIGHTING);
        SetPerspectiveProjectionMatrix();

        std::cout<< "\nbout de init()"<< std::endl;
        return true;
        }
    //mode sensor
    else if (mode == MODE_SENSOR){
     SetOrthoProjectionMatrix();
     return sensor.OnInit(true);// true-->color
    }
}

void CAvatar::OnCleanup(){
    SDL_FreeSurface(Surf_Test);
    if(sdl_pimage)
    SDL_FreeSurface(sdl_pimage);
    SDL_Quit();
}

void CAvatar::OnLoop(){}

void CAvatar::DrawDemo(){
    if(!needs_rendering) return;
    needs_rendering = false;



GLfloat scaling[] = {1,0,0,0,
                   0,1.5,0,0,
                   0,0,1,0,
                   0,0,0,1

                 };

//choose the matrix stack which we'll apply the coming actions, threes types:GL_PROJECTION, GL_MODELVIEW, GL_TEXTURE.
//au color
glMatrixMode((GL_MODELVIEW));
//reset the top matrix as unit matrix
glLoadIdentity();

glTranslatef(-camera_tx,-camera_ty,-camera_tz);
//rotate "world_rx" degree round axis x
glRotatef(world_rx,1,0,0);
glRotatef(world_ry,0,1,0);
glMultMatrixf(scaling);

//pour mode pespective, les deux ligns suivant remplacés par afficher surface
DrawFrame(world_origin_x,world_origin_y,world_origin_z,RDR_FRAME_LENGTH);
DrawCube(world_origin_x,world_origin_y,world_origin_z,RDR_CUBE_HALF_SIDE,texture);

}

void CAvatar::DrawSensor(){
    openni::VideoFrameRef m_colorFrame;
    //Lire un prochain frame de ce videostream, et le garde comme une VideoFrameRef.
    sensor.m_colorStream.readFrame(&m_colorFrame);
    if(!m_colorFrame.isValid()) {
        std::cout << "Invalid color frame" << std::endl;
        return;
    }
    const openni::RGB888Pixel* pImage = (const openni::RGB888Pixel*)m_colorFrame.getData();

    switch(sensor.active_stream) {
    /*pour afficher à l'écran: PImage,
    Glunit texID = Load2Dtexture(PImage),
    Fillwindow > fonction a ecricre qui dessine un rectangle de la taille de la fenetre et y colle la texture
    libérer: glDeleteTexture(1,&texID). sinon, accumuler les texture sur les mémoires.*/

    //mode ortho
    case color_stream:
        //l'accès à l'image
         //creer une texture à partir de pImage
         textureColor = Load2DTexture(m_colorFrame.getWidth(),m_colorFrame.getHeight(),3,pImage);
         //pour tracer un GL_QUADS occupant toute la fenête.
         //CAvatar::SetOrthoProjectionMatrix();
         //passer textureColor à fonction FillWindowWithTexture
         FillWindowWithTexture(textureColor);
         //libérer texture
         glDeleteTextures(1,&textureColor);
           break;


    case depth_stream:
        openni::VideoFrameRef m_depthFrame;
        sensor.m_depthStream.readFrame(&m_depthFrame);
        const openni::DepthPixel* pDepth = (const openni::DepthPixel*)m_depthFrame.getData();

        //effacer les buffers couleur et profondeur d'OpenGL,acceder aux images couleur et profondeur courantes

        int width = m_depthFrame.getWidth();
        int height = m_depthFrame.getHeight();
        //utilisés pour mettre la profondeur à m'échelle ci-dessous

        float pWorldX, pWorldY, pWorldZ;
        //définissez la matrice ModelView
        //Applies subsequent matrix operations to the modelview matrix stack.
        //glMatrixMode((GL_MODELVIEW));
        //glLoadIdentity();
        GLfloat scaling[] = {1,0,0,0,
                           0,1.5,0,0,
                           0,0,1,0,
                           0,0,0,1

                         };

        //choose the matrix stack which we'll apply the coming actions, threes types:GL_PROJECTION, GL_MODELVIEW, GL_TEXTURE.
        //au color
        glMatrixMode((GL_MODELVIEW));
        //reset the top matrix as unit matrix
        glLoadIdentity();

        //shift (0,0,-2)
        glTranslatef(-camera_tx,-camera_ty,-camera_tz);
        //rotate "world_rx" degree round axis x
        glRotatef(world_rx,1,0,0);
        glRotatef(world_ry,0,1,0);
        glMultMatrixf(scaling);

        glEnable(GL_DEPTH_TEST);
        glPointSize(2);
        //une bloque pour dessiner des points
        glBegin(GL_POINTS);
        for(int y=0; y<height;y++){
            for(int x=0;x<width;x++){
                if((x%2==0)&&(y%2==0)&&(*pDepth !=0)&&(*pDepth<2000)){
                    openni::CoordinateConverter::convertDepthToWorld(sensor.m_depthStream,x,y,*pDepth,&pWorldX,&pWorldY,&pWorldZ);
                    //définir la couleur du point 3D
                    glColor3f(pImage->r, pImage->g, pImage->b);
                    glVertex3f(pWorldX/1000.0,pWorldY/1000.0,pWorldZ/1000.0);

                }
                pDepth++;
                pImage++;
            }
        }
        glEnd();
        break;
    }
}
/*
* Draw objets
*/
void CAvatar::OnRender(int mode){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

if(mode == MODE_DEMO) DrawDemo();
else if(mode == MODE_SENSOR) DrawSensor();

SDL_GL_SwapBuffers();

}

void CAvatar::OnEvent(SDL_Event* Event){
    CEvent::OnEvent(Event);
    switch (Event->type){
      case SDL_QUIT:

         break;
        //key events
      case SDL_KEYDOWN:
         OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod, Event->type);
         break;
        //mouse events
      case SDL_MOUSEMOTION: {
               OnMouseMove(Event->motion.x,Event->motion.y,Event->motion.xrel,Event->motion.yrel);
               break;
           }

       case SDL_MOUSEBUTTONDOWN: {
               switch(Event->button.button) {
                   case SDL_BUTTON_LEFT: {
                       OnLButtonDown(Event->button.x,Event->button.y);
                       break;
                   }
                   case SDL_BUTTON_RIGHT: {
                       OnRButtonDown(Event->button.x,Event->button.y);
                       break;
                   }
                   case SDL_BUTTON_WHEELDOWN: {
                       OnMouseWheel(false,true);
                     break;
                   }

               }
               break;
           }

          case SDL_MOUSEBUTTONUP:    {
               switch(Event->button.button) {
                   case SDL_BUTTON_LEFT: {
                       OnLButtonUp(Event->button.x,Event->button.y);
                       break;
                   }
                   case SDL_BUTTON_RIGHT: {
                       OnRButtonUp(Event->button.x,Event->button.y);
                       break;
                   }
                   case SDL_BUTTON_WHEELUP:{
                        OnMouseWheel(true,false);
                   }
                 }
                 break;
           }


}
}

void CAvatar::InitSceneConstants(){
    world_rx = 0;
    world_ry = 0;

    camera_min_tz = world_origin_z +CAMERA_Z_OFFSET; // 2
    camera_tx = world_origin_x;
    camera_ty = world_origin_y;
    camera_tz = camera_min_tz;  // 2
}

void CAvatar::SetPerspectiveProjectionMatrix(){
   // use the projection matrix before drawing the objects in your scene to set the view volume.
   glMatrixMode(GL_PROJECTION);
   // replace the current matrix with the identity matrix
   glLoadIdentity();
   //set up a perspective projection matrix
   gluPerspective(camera_fovy,camera_aspect_ratio,camera_min_z,camera_max_z);
}

//event of press down the buttons of keyboard
void CAvatar::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode){
    switch (sym) {
    //Exit the programme
    case SDLK_ESCAPE:
        std::cout<< "\nyou've pressed ESC"<< std::endl;
        OnExit();
        break;
    //go back to original state
    case SDLK_SPACE:
        std::cout<< "\nyou've pressed ESPACE"<< std::endl;
        InitSceneConstants();
        needs_rendering = true;
        break;
    case SDLK_LEFT:
        std::cout<< "\nyou've pressed LEFT"<< std::endl;
        if(mod & KMOD_SHIFT){
            //rotate round axis y by 5° each time
            world_ry -= SCENE_ROTATION_STEP;
            std::cout<< "world_ry = "<<world_ry << camera_tx << std::endl;
            if(world_ry < 0)
                world_ry += 360;
        }else
            camera_tx -= CAMERA_TRANSLATION_STEP;
         std::cout<< "camera_tx = " << camera_tx << std::endl;
        needs_rendering = true;
        break;
    case SDLK_RIGHT:
        if(mod & KMOD_SHIFT){
            //rotate round axis y
            world_ry += SCENE_ROTATION_STEP;
             std::cout<< "world_ry = "<<world_ry << camera_tx << std::endl;
        }else
        //with glTranslatef(-camera_tx...) in OnRender(), the cube will go to left
            camera_tx += CAMERA_TRANSLATION_STEP;
         std::cout<< "camera_tx = " << camera_tx << std::endl;
        needs_rendering = true;
        break;
    case SDLK_DOWN:
        if(mod & KMOD_SHIFT){
            world_rx += SCENE_ROTATION_STEP;
        }else
            camera_ty += CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;
    case SDLK_UP:
        if(mod & KMOD_SHIFT){
            world_rx -= SCENE_ROTATION_STEP;
            if(world_rx < 0)
                world_rx += 360;
        }else
            camera_ty -= CAMERA_TRANSLATION_STEP;
        needs_rendering = true;
        break;
    case SDLK_q:
        // move outside, corresponding positif value of axis z
        camera_tz -= CAMERA_TRANSLATION_STEP;
         std::cout<< "camera_tz = "<< camera_tz << std::endl;
            //can't be bigger than the original size
        if(camera_tz < camera_min_tz)
            camera_tz = camera_min_tz;
        needs_rendering = true;
        break;
    case SDLK_a:
        //change z, mean move inside
        camera_tz += CAMERA_TRANSLATION_STEP;
         std::cout<< "camera_tz = " << camera_tz << std::endl;
        needs_rendering = true;
        break;
        //couleur
    case SDLK_c:
        InitSceneConstants();
        glMatrixMode((GL_MODELVIEW));
        //reset the top matrix as unit matrix
        glLoadIdentity();

        SetOrthoProjectionMatrix();
        sensor.active_stream = color_stream;
        break;
        //profondeur
    case SDLK_p:
        InitSceneConstants();
        SetPerspectiveProjectionMatrix();
        sensor.active_stream = depth_stream;
        break;
    }
}

void CAvatar::OnResize(int w, int h){
   window_width = w;
   window_height = h;

   SDL_FreeSurface(sdl_pimage);
   sdl_pimage = SDL_SetVideoMode(window_width, window_height, SDL_DEPTH, SDL_VIDEO_MODE_OPTIONS);
   glViewport(0,0,window_width,window_height);

   camera_aspect_ratio = ((float)window_width)/((float)window_height);
   SetPerspectiveProjectionMatrix();

   needs_rendering = true;
}

void CAvatar::OnExpose(){

    // à compléter
}

void CAvatar::OnExit(){
should_be_running = false;
}

void CAvatar::OnLButtonDown(int x, int y){
    boutonL = true;
}

void CAvatar::OnRButtonDown(int x, int y){
     boutonR = true;
}

void CAvatar::OnLButtonUp(int x, int y){
    boutonL = false;
}

void CAvatar::OnRButtonUp(int x, int y){
    boutonR = false;
}

void CAvatar::OnMouseMove(int x, int y, int xrel, int yrel){

    if (boutonR){ //si on veut se deplacer(clic droit)
       //deplace a droit
         std::cout<< xrel<< "&& "<<x << camera_tz << std::endl;
       if(xrel>0){
           camera_tx -= CAMERA_TRANSLATION_STEP;
           needs_rendering = true;
       }else if(xrel<0){
           camera_tx += CAMERA_TRANSLATION_STEP;
           needs_rendering = true;
       }
       if(yrel>0){
           camera_ty += CAMERA_TRANSLATION_STEP;
           needs_rendering = true;
       }else if(yrel<0){
           camera_ty -= CAMERA_TRANSLATION_STEP;
           needs_rendering = true;
       }
    }
    if (boutonL){ //si je veux rotation
        if(xrel>0){
            world_ry += SCENE_ROTATION_STEP;
             needs_rendering = true;
        }else if(xrel<0){
            world_ry -= SCENE_ROTATION_STEP;
            if(world_ry < 0)
                world_ry += 360;
             needs_rendering = true;
        }
        if(yrel>0){
            world_rx += SCENE_ROTATION_STEP;
             needs_rendering = true;
        }else if(yrel<0){
            world_rx -= SCENE_ROTATION_STEP;
           if(world_rx < 0)
               world_rx += 360;
              needs_rendering = true;
        }
    }
}

//to get bigger or smaller
void CAvatar::OnMouseWheel(bool Up, bool Down){
     if(Up==true){
         // move outside, corresponding positif value of axis z
         camera_tz -= CAMERA_TRANSLATION_STEP;
             //can't be bigger than the original size
         if(camera_tz < camera_min_tz)
             camera_tz = camera_min_tz;
         needs_rendering = true;
     }
     else if(Down ==true){
         camera_tz += CAMERA_TRANSLATION_STEP;
         needs_rendering = true;
     }
}

void CAvatar::SetOrthoProjectionMatrix(){
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
//multiply the current matrix with an orthographic matrix,(left,right,bottom,top,nearVal,farVal)
glOrtho(0,1,0,1,-10,10);

}
