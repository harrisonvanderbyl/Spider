#include "hitbox.h"
#include <thread>
void* load(std::string filename,IndexedModel* model,bool* done)
{
*model = OBJModel(filename).ToIndexedModel();
*done = true;
}
Hitbox::Hitbox(std::string filename)
{

    temp = new std::thread(load,filename,&m_model,&done);
}

Hitbox::~Hitbox()
{

}


void collison(int plafrom, int plato,Hitbox* me)
{

for(unsigned int face = plafrom; face < plato; face+=3)
{

glm::vec3 m1 = (me->m_model.positions[me->m_model.indices[face+0]]);
glm::vec3 n1 =me->m_model.normals[me->m_model.indices[face+0]];// glm::normalize(glm::vec3(me->mymat*glm::vec4(me->m_model.normals[me->m_model.indices[face]]+me->m_model.positions[me->m_model.indices[face+0]],1.0))-m1);




/**/
    float dista = glm::distance(me->from,me->to);

    //determins perpendicular distance to plane of polygon
    float perpendicular_distance = glm::dot(me->from-m1,n1);
    //discard following if all normals are facing the correct way, gives unknown performance boost if deleted
    //if (perpendicular_distance < 0.0){
       // perpendicular_distance = -perpendicular_distance;
       // n1=-n1;
    //}


    glm::vec3 check_vector = glm::normalize(me->to-me->from);
    //determins if possibility of intersection exists
    if (perpendicular_distance<dista){

        if (perpendicular_distance > glm::dot(me->from-me->to,-n1)){





            float collision_distance = -1;
            float cosin = glm::dot(n1,check_vector);
            //prevents /0 errors
            if (cosin!=0){
                //determins collision distance
                collision_distance = -perpendicular_distance/cosin;
            }


            if ((collision_distance > 0)){

            if (collision_distance < me->info[0].y){
            if ((collision_distance < dista)){
                //locates intersection on plane
                glm::vec3 in = me->from+check_vector*collision_distance;

                glm::vec3 m2 = (me->m_model.positions[me->m_model.indices[face+1]]);
                glm::vec3 m3 = (me->m_model.positions[me->m_model.indices[face+2]]);




                float area =
                glm::length(glm::cross(m1-in,m2-in))+
                glm::length(glm::cross(m3-in,m2-in))+
                glm::length(glm::cross(m1-in,m3-in))-
                glm::length(glm::cross(m1-m2,m3-m2));



                if (glm::abs(area)<0.001f)
                {

             //me->info[0].z = me->info[0].z + 1.0;
               me-> info[0] = glm::vec3(0.0,collision_distance,me->info[0].z);
                me->info[1] = in;
               me-> info[2]=n1;
               me-> info[3]=in+n1*perpendicular_distance;
                }



}
}}
}}}
}

std::vector<glm::vec3>  Hitbox::checkCol(const glm::vec3& fromo,const glm::vec3& too,const glm::mat4 mymato)
{
mymat = glm::inverse(mymato);
from = glm::vec3(mymat*glm::vec4(fromo,1.0));
to = glm::vec3(mymat*glm::vec4(too,1.0));;
info.clear();
info.push_back(glm::vec3(1.0,100000.0,0.0));// hit(),distance(), null()
info.push_back(to);//position of impact
info.push_back(glm::vec3());//normal of impact
info.push_back(to);//corrected local
if(!done){return info;}
//temp->join();
int threadsize = 60;
int threads = int(float(m_model.indices.size())/float(threadsize))+1;
std::vector<std::thread> threadss;
for(int i = 0; i < threads; i++)
{

threadss.push_back(std::thread(collison,i*threadsize,(int)glm::min(float((i+1)*threadsize),float(m_model.indices.size())),this));


}
for(int i = 0; i < threads; i++)
{

  threadss[i].join();


}

info[2]=glm::vec3(glm::inverse(mymat) * glm::vec4(info[2]+info[1],1.0));
info[1]=glm::vec3(glm::inverse(mymat) * glm::vec4(info[1],1.0));
info[2]-=info[1];
info[3]=glm::vec3(glm::inverse(mymat) * glm::vec4(info[3],1.0));
//collison(0,m_model.indices.size(),this);
return info;
}
