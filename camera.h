#ifndef CAMERA_H
#define CAMERA_H
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include "hitbox.h"
class Camera
{
public:
    Camera(const glm::vec3& pos,float fov,float aspect, float znear, float zfar,const Uint8* mov )
    {
        //SDL_SetRelativeMouseMode(SDL_TRUE);
        //SDL_CaptureMouse(SDL_TRUE);
        m_mov = mov;
        zNear = znear;
        zFar = zfar;
       // m_perspective = glm::ortho(-100.0f,100.0f,-100.0f,100.0f,zNear,zFar) ;//
      m_perspective = glm::perspective(fov,aspect,zNear,zFar);
        m_position = pos;
        m_forward = glm::vec3(0,0,1);
        m_up = glm::vec3(0,1,0);


    }

    inline glm::mat4 GetViewProjection() const
    {
        return m_perspective * glm::lookAt(m_position,m_position+m_forward,m_up);
    }
    inline glm::vec3& pos()
    {
        return m_position;
    }
    inline void update()
    {
        int x;
        int y;
        SDL_GetGlobalMouseState(&x,&y);
        m_aangle = float((x)-(400))/1000.0;
        glm::vec3 fl = (m_forward - m_up * glm::dot(m_up,m_forward));

        m_yaangle =  glm::clamp(m_yaangle-float((y)-(500))/1000.0,-3.1415/2.0,3.1415/2.0);
        glm::vec3 fd = -glm::normalize(glm::cross(m_up,glm::vec3(1.0,0.0,0.0)));
        glm::vec3 ld = glm::normalize(glm::cross(m_up,fd));
        m_aangle += glm::atan(glm::dot(fl,fd),glm::dot(fl,ld))-3.1415f/2.0f;
        m_forward = glm::sin(m_yaangle)*m_up+-glm::sin(m_aangle)*glm::cos(m_yaangle)*ld+glm::cos(m_aangle)*glm::cos(m_yaangle)*fd;//glm::vec3(-sin(m_aangle)*cos(m_yaangle),sin(m_yaangle),cos(m_aangle)*cos(m_yaangle));
        SDL_WarpMouseGlobal(400,500);
        time+=1.0;


    };
    inline void passkey(Hitbox* hit,const glm::mat4 check)
    {

        update();

        //mov[MOUSEX]=0;

           //movem;
       // m_position -= glm::vec3(0.0,1.0,0.0);
      //  glm::vec3 old = m_position ;
      float movspeed = 0.1;
        glm::vec3 mol;
          if (m_mov[SDL_SCANCODE_SPACE] == 1)
        {
           mol+= m_up*movspeed;

        }
          if (m_mov[SDL_SCANCODE_LSHIFT] == 1)
        {
           mol-= m_up*movspeed;

        }
        if (m_mov[SDL_SCANCODE_W] == 1)
        {
           mol+= m_forward*movspeed;

        }
        if (m_mov[SDL_SCANCODE_S] == 1)
        {
           mol += -m_forward*movspeed;
        }
        if (m_mov[SDL_SCANCODE_A] == 1)
        {
            mol -= glm::normalize(glm::cross(m_forward,m_up))*movspeed;

        }
        if (m_mov[SDL_SCANCODE_D] == 1)
        {
           mol += glm::normalize(glm::cross(m_forward,m_up))*movspeed;

        }

      //  mol.y = (1.0-mov.y)/10.0;

        std::vector<glm::vec3> che;
        che =  hit->checkCol(m_foot+mov,m_foot+mov+mol,check);

        mov=che[3]-m_foot;

        //mov = (che[1]-(m_foot));

        if (glm::length(mov)>1.01){

        che= hit->checkCol(m_foot+mov,m_foot+mov+glm::vec3(0.0,-100.0,0.0),check);
       mov = mov+m_foot;
        m_foot = che[1]+glm::vec3(0.0,0.01,0.0);
        mov = mov - m_foot;

        }

      /*  m_vel =(m_vel+ 9.8/5000.0) * che[0].x;
        if (che[0].x==0){
       che = hit->checkCol(old,old+glm::normalize(che[1]+glm::reflect(che[1]-old,che[2])-old)*0.1f,check);}
          if (che[0].x==0){
       che = hit->checkCol(old,old+glm::normalize(che[1]+glm::reflect(che[1]-old,che[2])-old)*0.1f,check);}
*/

        //time = glm::fract(time/3.1415f)*3.1416;

        m_position = m_foot+mov;//glm::normalize(che[1]+glm::reflect(che[1]-old,che[2])-old)*0.1f +glm::vec3(0.0,1.0,0.0);











    }
    inline glm::vec3 getposition() const{return m_position;}
    inline glm::vec3 getforward() const{return m_forward;}
    inline float getang() const{return m_aangle;}
 inline float getyang() const{return m_yaangle;}
 //inline mat4 getp();
    std::vector<glm::vec4> lights;
    glm::vec3 m_position;
    glm::vec3 m_forward;
    float m_aangle = 0.0f;
    float m_yaangle ;
    float time = 0.0;
    int cover = 0;
    glm::vec3 m_up;
    glm::mat4 m_perspective;
    float zNear;
    float zFar;
    glm::vec3 m_foot;
    glm::vec3 mov;
     const Uint8* m_mov;
    enum
    {
        UP,
        DOWN,
        LEFT,
        RIGHT,
        MOUSEX,
        MOUSEY,
        WINDOWX,
        WINDOWY,
        WINDOWW,
        WINDOWH,
        MOUSED,
        JUMP,
        NUM_COMMANDS
    };
    float m_vel = 0.0f;
};

#endif // CAMERA_H
