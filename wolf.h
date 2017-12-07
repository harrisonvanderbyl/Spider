#ifndef WOLF_H
#define WOLF_H
#include "camera.h"
#include "body.h"
#include "extinput.h"
class Wolf
{
    public:
        Wolf(const Uint8* keys);
        void draw(Camera* camera);
        glm::vec3 norm = glm::vec3(0.0,1.0,0.0);
    Body* m_body;
    Body* m_legs;
    glm::mat4* m_trans = new glm::mat4(glm::scale(glm::vec3(1.0f,1.0f,1.0f)));

    void update(Camera* camera,Hitbox* hit, glm::mat4 check);

    void drawshadow(Camera* camera);
    float level = 1.0;
    glm::vec3 foot = glm::vec3(0.0);
    glm::vec3 moss = glm::vec3(0.0f,1.0f,0.0f);
    int lightindex = -1;
    glm::vec3 velocity = glm::vec3(0.0,0.0,0.0);
    int mem = 0;
    float anitime = 0.0f;
    glm::vec3 poss = glm::vec3(0.0);
    glm::vec3 normto;
    Extinput m_rotor;
    const Uint8* m_keys;
    int action = 0;
    float dir = 0.0;
    enum{
    ATTACK,
    };


};

#endif // WOLF_H
