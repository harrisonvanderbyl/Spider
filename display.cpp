#include "display.h"
#include <GL/glew.h>
#include <iostream>
Display::Display(int width, int height,const std::string& title)
{
    SDL_Init(SDL_INIT_EVERYTHING);
    int s = 1;
//     SDL_EnableUNICODE(1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,4);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,4);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,4);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,4);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    m_window = SDL_CreateWindow(title.c_str(),SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height, SDL_WINDOW_OPENGL);
    m_glcontext = SDL_GL_CreateContext(m_window);



    GLenum status = glewInit();
    if(status!=GLEW_OK)
    {

        std::cout << "glew failed init error";

    }
    m_isclosed = false;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);


    SDL_GetWindowPosition(m_window,&m_movement[WINDOWX],&m_movement[WINDOWY]);

    SDL_GL_GetDrawableSize(m_window,&m_movement[WINDOWW],&m_movement[WINDOWH]);
}
bool Display::is_closed()
{
    return m_isclosed;
}
Display::~Display()
{
    SDL_GL_DeleteContext(m_glcontext);

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}
void Display::Clear(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}
void Display::SwapBuffers()
{
   /*m_movement[ROTY]=0;
   m_movement[ROTX]=0;
   m_movement[ROTZ] = 0;
   m_movement[DISCARD]=0;
   m_movement[BONEPLACE]=0;
   m_movement[MOUSERIGHT]=0;
      m_movement[EXTEND]=0;
      m_movement[ATTACH]=0;*/
    SDL_Event e;
    while(SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            m_isclosed = true;
            break;

        case SDL_KEYUP:
            switch(e.key.keysym.sym)
            {

            case 27:
            m_isclosed = true;
             SDL_GL_DeleteContext(m_glcontext);
             SDL_DestroyWindow(m_window);
               SDL_Quit();
               break;




            //case SDL_MOUSEMOTION : m_movement[MOUSEX] = e.motion.x-400;break;
        }

    }

}
  SDL_GL_SwapWindow(m_window);
}
