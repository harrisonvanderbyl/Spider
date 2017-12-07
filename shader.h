#ifndef SHADER_H
#define SHADER_H
#include <string>
#include <GL/glew.h>
#include "transform.h"
#include "camera.h"
class Shader
{
public:
    Shader(const std::string& fileName,const std::string& frags = "./res/basicShader.fs");
    void Bind();
    virtual ~Shader();
    //void Update(const Transform& transform,const std::string& uName);
    void Setcam( const Camera& camera);

    void Update(const glm::mat4& model,const std::string& uName){glUseProgram(m_program);glUniformMatrix4fv(glGetUniformLocation(m_program,uName.c_str()),1,GL_FALSE,&model[0][0]);}


    void Update(const glm::vec3& model,const std::string& uName){glUseProgram(m_program);glUniform3fv(glGetUniformLocation(m_program,uName.c_str()),1,&model[0]);}
    void Update(const glm::vec4& model,const std::string& uName){glUseProgram(m_program);glUniform4fv(glGetUniformLocation(m_program,uName.c_str()),1,&model[0]);}

    void Update(const float model,const std::string& uName){glUseProgram(m_program);glUniform1f(glGetUniformLocation(m_program,uName.c_str()),model);}
    void Update(const int model,const std::string& uName){glUseProgram(m_program);glUniform1iARB(glGetUniformLocation(m_program,uName.c_str()),model);}

protected:

private:
    static const unsigned int NUM_SHADERS = 2;
    Shader(const Shader& other) {};
    void operator=(const Shader& other) {};
    std::string LoadShader(const std::string& fileName);
    void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
    GLuint CreateShader(const std::string& text, unsigned int type);

    enum
    {
        TRANSFORM_U,
        TRANSFORM_V,
        UNIFORM_CAM,
        UNIFORM_TIME,
        NUM_UNIFORMS,

    };


    GLuint m_program;
    GLuint m_shaders[NUM_SHADERS];
    GLuint m_uniforms[NUM_UNIFORMS];

};

#endif // SHADER_H
