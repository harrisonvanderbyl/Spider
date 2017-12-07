#include "shader.h"
#include <iostream>
#include <fstream>
std::string LoadShader(const std::string& fileName);
void CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage);
static GLuint CreateShader(const std::string& text, GLenum shaderType);

Shader::Shader(const std::string& fileName,const std::string& frags)
{
    m_program = glCreateProgram();
    m_shaders[0] = CreateShader(LoadShader(fileName),GL_VERTEX_SHADER);
    //m_shaders[1] = CreateShader(LoadShader("bone.ts"),GL_TESS_CONTROL_SHADER);
    m_shaders[1] = CreateShader(LoadShader(frags),GL_FRAGMENT_SHADER);
    for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {

        glAttachShader(m_program,m_shaders[i]);

        glBindAttribLocation(m_program,0,"position");
        glBindAttribLocation(m_program,1,"texco");
        glBindAttribLocation(m_program,2,"normal");
        glBindAttribLocation(m_program,3,"indecie");
        glBindAttribLocation(m_program,4,"weights");
        glBindAttribLocation(m_program,5,"indecie2");
        glBindAttribLocation(m_program,6,"weights2");
        glBindAttribLocation(m_program,7,"indecie3");
        glBindAttribLocation(m_program,8,"weights3");
        glLinkProgram(m_program);
        CheckShaderError(m_program,GL_LINK_STATUS,true,"error shader program failed to link");
        glValidateProgram(m_program);
        CheckShaderError(m_program,GL_VALIDATE_STATUS,true,"error invalid shader");

        m_uniforms[UNIFORM_TIME] = glGetUniformLocation(m_program,"time");
        m_uniforms[TRANSFORM_V] = glGetUniformLocation(m_program,"view");
        m_uniforms[UNIFORM_CAM] = glGetUniformLocation(m_program,"cama");

    }
}

Shader::~Shader()
{
    for(unsigned int i = 0; i < NUM_SHADERS; i++)
    {

        glDetachShader(m_program,m_shaders[i]);
        glDeleteShader(m_shaders[i]);
    }
    glDeleteProgram(m_program);
}




void Shader::Bind()
{
    glUseProgram(m_program);

}
void Shader::Setcam( const Camera& camera)
{
glUseProgram(m_program);
glm::vec3 campos = camera.getposition();
glm::mat4 view = camera.GetViewProjection();
glUniformMatrix4fv(m_uniforms[TRANSFORM_V],1,GL_FALSE,&view[0][0]);
glUniform3fv(m_uniforms[UNIFORM_CAM],1,&campos[0]);
glUniform1f(m_uniforms[UNIFORM_TIME],camera.time);
}

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
            output.append(line + "\n");
        }
    }
    else
    {
        std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::CheckShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cerr << errorMessage << ": '" << error << "'" << std::endl;
    }
}
/*GLuint Shader::CreateShader(const std::string& text, unsigned int type)
{
    GLuint shader = glCreateShader(type);

    if(shader == 0)
		std::cerr << "Error compiling shader type " << type << std::endl;

    const GLchar* p[1];
    p[0] = text.c_str();
    GLint lengths[1];
    lengths[0] = text.length();

    glShaderSource(shader, 1, p, lengths);
    glCompileShader(shader);

    CheckShaderError(shader, GL_COMPILE_STATUS, false, "Error compiling shader!");

    return shader;
}*/
GLuint Shader::CreateShader(const std::string& text,unsigned int shaderType)
{
    GLuint shader = glCreateShader(shaderType);

    if(shader == 0)
        std::cerr << "error out of mem"  ;


    const GLchar* shadersourcestrings[1];
    shadersourcestrings[0] = text.c_str();
    GLint shadersourcestringslength[1];
    shadersourcestringslength[0] = text.length();
    glShaderSource(shader,1,shadersourcestrings,shadersourcestringslength);
    glCompileShader(shader);
    CheckShaderError(shader,GL_COMPILE_STATUS,false,"error shader comp error");

    return shader;
}
