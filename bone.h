#ifndef BONE_H
#define BONE_H

#include <glm/glm.hpp>
#include "camera.h"
#include "shader.h"
#include "bone.h"
#include <iostream>
#include <string>
#include <fstream>
class Bone
{
public:
    Bone(Transform* t);
    void Draw(Camera camera);
    inline std::vector<Transform*>* Getbones()
    {
        return &m_bones;
    }
    inline std::vector<Transform*>* GetTemp()
    {
        return &temp;
    }
    inline std::vector<glm::vec3>* Getbonepos()
    {
        return &m_bonepos;
    }
    inline void animate(float framee,int motion,float ridgitity = 1.0f)
    {
        float mix = glm::fract(framee);
        int frame = (int(glm::floor(framee+1.0))%(int(m_animate[motion].size() - 1)));
        int frame1 = (int(glm::floor(framee))%(int(m_animate[motion].size() - 1)));
        for(unsigned int i = 0; i < m_channels.size(); i++)
        {
            glm::vec3 f1 = glm::vec3(m_animate[motion][frame1][i*3+0],m_animate[motion][frame1][i*3+1],m_animate[motion][frame1][i*3+2]);
            glm::vec3 f2 = glm::vec3(m_animate[motion][frame][i*3+0],m_animate[motion][frame][i*3+1],m_animate[motion][frame][i*3+2]);

            glm::vec3 prefix = ridgitity*(glm::mix(f1,f2,mix));
            if(m_PosRot[i]){prefix = (prefix/360.0f); }
            else{prefix = 0.25f*prefix; }
            *(m_channels[i]) = prefix;

        }

    }
    void save();
    inline std::vector<glm::vec3> getpositions()
    {
        std::vector<glm::vec3> tempo;
        for(int i = 0; i < m_bones.size(); i++)
        {
            tempo.push_back(*m_bones[i]->getb());
        }
        return tempo;
    }
    void load(std::string fname);

    void setdiagnosticbone(int i)
    {
        m_diagnosticbone = i;
    }
    void attach(Shader*);
    virtual ~Bone();
    std::vector<float> parsebones(const std::string& line1);
    float m_frametime = 0.1;

    int frame = 0;
    std::vector<int> chain;


    Transform* m_t;
    int m_diagnosticbone;
    std::vector<Transform*> m_bones;
    std::vector<glm::vec3> m_bonepos;
    std::vector<Transform*> temp;
    std::vector<glm::vec3*> m_channels;
    std::vector<bool> m_PosRot;
    std::vector<std::vector<std::vector<float>>> m_animate;
    //  Actor* arrow;
};

#endif // BONE_H
