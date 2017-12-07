#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glew.h>
class Texture
{
public:
    Texture(const std::string& fileName);
    void Bind(unsigned int unit);

    virtual ~Texture();
   // Texture(const Texture& other) {};
   // Texture& operator=(const Texture& other) {};
    GLuint m_texture;
protected:

private:


};

#endif // TEXTURE_H
