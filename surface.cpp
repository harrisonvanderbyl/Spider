#include "surface.h"

Surface::Surface(int width, int height, int depth)
{
    m_surface = SDL_CreateRGBSurface(SDL_SWSURFACE, m_width,m_height, 24*depth, 0x000000FF, 0x0000FF00, 0x00FF0000, 0);
    glGenTextures(1,&m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}


