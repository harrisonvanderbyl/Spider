#ifndef TRANSFORM_H
#define TRANSFORM_H
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
class Transform
{
public:
    Transform(const glm::vec3& pos = glm::vec3(0.0), const glm::vec3& rot = glm::vec3(0.0),const glm::vec3& scale = glm::vec3(1.0), glm::mat4* link = new glm::mat4(glm::scale(glm::vec3(1.0))),glm::vec3 place = glm::vec3(0.0),glm::vec3* linkp = new glm::vec3(0.0),glm::vec3* p = new glm::vec3(0.0),glm::mat4* bonelink = new glm::mat4(glm::scale(glm::vec3(1.0)))  )
    {
        this->m_defaultlink = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));
        this->m_pos = new glm::vec3(pos);
        this->m_rot = new glm::vec3(rot);
        this->m_scale = new glm::vec3(scale);
        this->m_link = link;
        this->m_blink = bonelink;
        this->m_model = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));
        this->m_bonmat = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));
        this->m_mymodel = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));
        this->m_aa = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));
        this->m_bonep = place;
        this->m_p = p;
        this->m_origin = *p;
        this->m_linkp = linkp;
        this -> m_linkpf = new glm::vec3(0.0);
        update();
    }
    inline glm::vec3* getb()
    {
//  *m_bonmat = *m_model;
        *m_p =  (glm::vec3(*m_bonmat * glm::vec4(m_origin,1.0)) )  ;

        return m_p;
    }
    inline void setlf(glm::vec3* a)
    {
//  *m_bonmat = *m_model;

        m_linkpf = a;
    }
    inline glm::mat4* getMatp()
    {
        update();
        return m_model;
    }
    inline glm::mat4* getMatbone()
    {
        update();

        //*m_bonmat =   glm::translate(m_bonep)   ;

        *m_bonmat = *m_blink*glm::translate(m_origin)**m_mymodel * glm::translate(-m_origin)**m_linka;
        // *m_aa = glm::translate(*getb()/2.0f)* *m_model * glm::translate(-*getb()/2.0f);
        // std::cout << glm::distance(*getb(),glm::vec3(*m_defaultlink * glm::vec4(*getb(),1.0))) <<"\n";*glm::scale(glm::vec3(0.0001))


        return m_bonmat;
    }
    inline glm::mat4* getMat()
    {
        *m_model = (*(m_link)) **m_mymodel;
        //update();
        return m_model;
    }
    inline glm::mat4* getMod()
    {
//update();
        return  m_mymodel;
    }

    inline glm::vec3* getpp()
    {
        // *m_p = *m_linkp + glm::vec3(glm::vec4(m_bonep,1.0) * glm::inverse(*m_link)) ;
//*m_p =   *m_linkp+glm::vec3((*m_link) *glm::vec4( m_bonep,1.0))  ;

        return m_linkp;
    }
    inline glm::mat4* breakLink()
    {
        *m_defaultlink = *m_link;
        m_link = m_defaultlink;
        return m_link;
    }
    void update()
    {
        //  m_pos = glm::vec3(*m_bonmat * glm::vec4(0.0,0.0,0.0,1.0)) ;
        glm::vec3 a = ((*m_pos));
        glm::mat4 posMatrix = glm::mat4(glm::translate(a));
        glm::mat4 rotXMatrix = glm::rotate((m_rot->x), glm::vec3(1.0,0.0,0.0));
        glm::mat4 rotYMatrix = glm::rotate((m_rot->y), glm::vec3(0.0,1.0,0.0));
        glm::mat4 rotZMatrix = glm::rotate((m_rot->z),  glm::vec3(0.0,0.0,1.0));
        glm::mat4 scaleMatrix = glm::scale(*m_scale);

        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
        //*model =  *m_link * posMatrix * rotMatrix * scaleMatrix;

        *m_mymodel = posMatrix*rotMatrix*scaleMatrix;
        *m_model =   (*(m_link))**m_mymodel;



    }


    glm::mat4* m_bonmat;
    glm::mat4* m_link;
    glm::vec3* m_pos;
    glm::vec3* m_rot;
    glm::vec3* m_scale;
    std::string name = "";

    //        Transform(const Transform& other){};
    //    void operator=(const Transform& other){};
    glm::vec3 m_bonep;

    glm::mat4* m_blink;
    glm::mat4* m_model;
    glm::mat4* m_mymodel;
    glm::mat4* m_aa;
    glm::mat4* m_defaultlink;
    glm::vec3* m_linkp;
    glm::mat4* m_linka = new glm::mat4(glm::scale(glm::vec3(1.0f)));
    glm::vec3 m_origin;
    glm::vec3* m_p;
    glm::vec3* m_linkpf;
};

#endif // TRANSFORM_H
