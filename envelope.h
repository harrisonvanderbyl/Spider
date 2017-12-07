#ifndef ENVELOPE_H
#define ENVELOPE_H
#include "actor.h"
#include "shader.h"


class Envelope
{
    public:
        Envelope(int* keys,std::string letternum, Transform* chain = new Transform()){
        m_t = new Transform(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(1.0),chain->getMatp());
        m_flap = new Actor("./res/letterflap.obj","./res/letterflap.obj","./res/letter.jpg","./res/letter.vs","./res/basicShader.fs",false,m_t);
        m_body = new Actor("./res/letter.obj","./res/letter.obj","./res/letter.jpg","./res/letter.vs","./res/basicShader.fs",false,m_t);
        m_paper = new Actor("./res/paper.obj","./res/paper.obj","./res/letter"+letternum+".jpg","./res/paper.vs","./res/basicShader.fs",false,m_t);
        *(m_paper->m_transform->m_scale)=(glm::vec3(0.09));
        *(m_body->m_transform->m_scale)=(glm::vec3(0.1));
        *(m_flap->m_transform->m_scale)=(glm::vec3(0.1));
        m_keys = keys;
        m_opened = 0.0;
        };
        void Draw(const Camera& camera){
        *(m_t->m_scale) = (glm::vec3(1.0));
        *(m_flap->m_transform->m_rot) = (glm::vec3(glm::clamp(m_open,-3.1415f/2.0f,0.0f)*2.0f,0.0,0.0));


        m_selected = 1.0-m_body->m_hitbox->checkCol(camera.getposition(),camera.getposition()+camera.getforward(),*(m_body->m_transform->getMat()))[0].x;

        m_body->draw(camera);
        m_body->Shad()->Update(m_selected,"selected");
        m_flap->draw(camera);
        m_flap->Shad()->Update(m_selected,"selected");
        m_open-=m_opened;
        if (m_open > 0.0){
        m_opened = 0.0;
        m_open = 0.0;

        }
        if ((m_selected==1.0)&&(m_keys[10]==true)){
        m_opened = 0.05;
        if (m_opened!=0){m_open = 0.0;}
        }


        if (glm::abs(m_open)>0.0)
        {
        if(m_keys[16]){
        m_opened = -0.05;
        m_open = -3.1415*2.0;
        }
        //if (m_open < -3.1415){-3.1415
        //m_paper->getmat()->breakLink();}
        //m_paper->getmat()->SetScale(glm::vec3(m_open/(-3.1415f)));

       // (,,-glm::clamp((m_open+3.1415)/2.0f,-1.0,0.0)));
         //->SetRot(glm::vec3(0.0,0.0,3.1415));
      //  show.SetPos( camera.getposition()+camera.getforward()*0.15f+glm::vec3(0.0,0.05,0.0));
       // show.SetRot(glm::vec3(3.1415f/2.0f,-camera.getang(),0.0f));
       // show.SetScale(glm::vec3(0.1));
       // hide.SetPos(glm::vec3(0.0,0.0005,0.1*glm::clamp((m_open+3.1415f/2.0f),-1.0f,0.0f)));
       // hide.SetRot(glm::vec3(0.0,0.0,3.1415));
       // hide.SetScale(glm::vec3(0.1));
        *(m_paper->m_transform->m_pos)=(glm::mix(glm::vec3(0.0,0.0005,0.1*glm::clamp((m_open+3.1415f/2.0f),-1.0f,0.0f)),camera.getposition()+camera.getforward()*0.15f+glm::vec3(0.0,0.05,0.0),-glm::clamp((m_open+3.1415)/2.0f,-1.0,0.0)));
        *(m_paper->m_transform->m_pos)=(glm::mix(glm::vec3(0.0,0.0,3.1415),glm::vec3(3.1415f/2.0f,-camera.getang(),0.0f),-glm::clamp((m_open+3.1415)/2.0f,-1.0,0.0)));
        *(m_paper->m_transform->breakLink()) = glm::mix(*(m_t->getMat()),glm::mat4(glm::scale(glm::vec3(1.0))),-glm::clamp((m_open+3.1415)/2.0f,-1.0,0.0));

        //*(m_paper->getmat()->getMod()) = glm::mix(*(hide.getMatp()),*(show.getMatp()),-glm::clamp((m_open+3.1415)/2.0f,-1.0,0.0));



        m_paper->draw(camera);
        m_paper->Shad()->Update(-glm::clamp(m_open+3.1415f,-1.0f,0.0f),"selected");
        }

        }
        inline Transform* getMod(){return m_t;}

        virtual ~Envelope(){};
    protected:
    private:
    Actor* m_flap;
    Actor* m_body;
    Actor* m_paper;
    float m_selected;
    float m_open;
    float m_opened;
    Transform show;
    Transform hide;
    Transform* m_t;
    int* m_keys;
};

#endif // ENVELOPE_H
