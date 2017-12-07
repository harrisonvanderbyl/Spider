#ifndef SURFACE_H
#define SURFACE_H

#include <SDL2/SDL.h>
#include <GL/glew.h>

class Surface
{
    public:
        Surface(int width, int height, int depth = 1);
        ~Surface(){SDL_FreeSurface(m_surface);}
void Draw()
{



}
void Settex(int i)
{

            glActiveTexture(GL_TEXTURE0+i);
            glBindTexture(GL_TEXTURE_2D,m_texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024 , 1024, 0, GL_RGB, GL_UNSIGNED_BYTE,m_surface->pixels);

}
void Setsurface()
{


}
void Resetsurface()
{
glReadPixels(0, 0,m_width ,m_height , GL_RGB, GL_UNSIGNED_BYTE, m_surface->pixels);

}
    int m_width;
    int m_height;
    GLuint m_texture;
    SDL_Surface* m_surface;
};

#endif // SURFACE_H
