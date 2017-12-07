#ifndef HITBOX_H
#define HITBOX_H
#include <vector>
#include "obj_loader.h"
#include <glm/glm.hpp>
#include "transform.h"
#include "mesh.h"
#include <thread>
class Hitbox
{
    public:
        Hitbox(std::string filename);
        virtual ~Hitbox();
        std::vector<glm::vec3> checkCol(const glm::vec3& fromo,const glm::vec3& too,const glm::mat4 mymato);
        float checkoverlap(Hitbox* other,const glm::mat4 othermat,const glm::mat4 mymat);
        inline IndexedModel getMod() const{return m_model;}
   // void collison(int plafrom, int plato);

       glm::vec3 to;
       glm::vec3 from;
       glm::mat4 mymat;
       std::thread *temp;
       bool done = false;
        IndexedModel m_model;
        std::vector<glm::vec3> info;
};

#endif // HITBOX_H
