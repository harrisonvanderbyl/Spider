#ifndef BODY_H
#define BODY_H
#include "actor.h"
#include <string>
#include "bone.h"
class Body
{
    public:
        Body(const std::string& meshname,const std::string& hitboxname,const std::string& texname,const std::string& shadername,const std::string& bonename);
        virtual ~Body();
        inline Actor* getbody(){return m_skin;}
        inline Bone* getbone(){return m_bones;}
        void bone()
        {
          m_skin->bone(m_bones);
          //m_skin->getmesh()->reinit();
          }
          int findbone(Camera camera){
         // glm::vec3 from = camera.getposition();
        //  glm::vec3 to = camera.getposition()+camera.getforward();
        //  *(m_arrow->getmat()->breakLink())=glm::inverse(glm::rotate(3.1415f/2.0f,glm::vec3(1.0,0.0,0.0))*glm::lookAt(from,to,glm::vec3(0.0,1.0,0.0)))*glm::translate(glm::vec3(0.0,0.5,0.0));//*(m_bones->Getbones()[0][i]->getMatbone(m_bones->Getbonepos()[0][i]))*glm::translate(m_bones->Getbonepos()[0][i]);

          //      m_arrow->draw(camera);
          int tf = -1;
          float as = 100000;
          for(unsigned int i = 0; i < m_bones->Getbones()[0].size();i++){
glm::vec3 basepos = *(*m_bones->Getbones())[i]->getpp();//(*m_bones->Getbonepos())[i];//*(*m_bones->Getbones())[i]->getpp();
//std::cout << m_bones->chain[i];
glm::vec3 tippos = *(*m_bones->Getbones())[i]->getb();//*(*m_bones->Getbones())[i]->getb(); //glm::vec3(glm::vec4(,1.0f)*(glm::inverse(glm::translate(basepos)*base*glm::translate(-basepos))));
glm::mat4 sca = glm::scale(glm::vec3(1.0f,1.0f,1.0f)*glm::vec3((glm::length(basepos-(tippos)))));
//glm::translate(tippos)* *(*m_bones->Getbones())[i]->m_link
*(m_arrow[i]->m_transform->breakLink())=glm::inverse(glm::rotate(3.1415f/2.0f,glm::vec3(1.0,0.0,0.0))*glm::lookAt(basepos,tippos,glm::vec3(0.0,1.0,0.0)))*sca*glm::translate(glm::vec3(0.0,0.5,0.0));//*(m_bones->Getbones()[0][i]->getMatbone(m_bones->Getbonepos()[0][i]))*glm::translate(m_bones->Getbonepos()[0][i]);
       // glm::vec3 ll = glm::vec3(glm::vec4(tippos,1.0) * camera.GetViewProjection());
        float diss = glm::distance(tippos,camera.getposition()+camera.getforward()/3.0f);
        float ia = float((diss<0.125f) );
        m_arrow[i]->Shad()->Update(float(ia),"selected");
        m_arrow[i]->draw(camera);

 //m_arrow->Shad()->Update(float(1.0),"selected");
         if((ia)&&(diss < as)){tf = i;as = diss;}

          }
          return tf;
          }
        void draw(Camera Camera);
        void drawshadow(Camera camera);
    Bone* m_bones;
    Transform* m_transform;
    protected:
    private:
    Actor* m_skin;
    std::vector<Actor*> m_arrow;
};

#endif // BODY_H
