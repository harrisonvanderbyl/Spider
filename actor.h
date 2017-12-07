#ifndef ACTOR_H
#define ACTOR_H
#include "mesh.h"
#include "hitbox.h"
#include "texture.h"
#include <string>
#include <vector>
#include "shader.h"
#include "transform.h"
#include "obj_loader.h"
#include "bone.h"
#include <thread>



void inito(Hitbox**,Mesh** m_mesh, Mesh** m_shadow, std::string fileName,std::string hitName, bool hasshadow, std::string ff, bool* done);

class Actor
{
    public:
        //Actor(const std::string& fileName,const std::string& hitName,const std::string& texName,const std::string& shadName);
        Actor(const std::string& fileName = "",const std::string& hitName = "", const std::string& texName = "", const std::string& shadName = "./res/basicShader.vs", const std::string& frags = "./res/basicShader.fs",bool hasshadow = true,Transform* trans = new Transform(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f)),const std::string& ff = "", Bone* bone = NULL)
       {
m_bone = bone;
m_hasshadow = hasshadow;
//m_mytransform = trans;
m_transform = trans;//new Transform(glm::vec3(0.0f),glm::vec3(0.0f),glm::vec3(1.0f,1.0f,1.0f),trans->getMat());//glm::vec3(),glm::vec3(),glm::vec3(1.0),m_mytransform->getModel());
//std::cout << ff;
worker = std::thread(inito,&m_hitbox,&m_mesh,&m_shadow,fileName,hitName,hasshadow,ff,done);
m_textures.push_back( new Texture(texName));
m_textures.push_back( new Texture("./res/stars.jpg"));

m_shader =  new Shader(shadName,frags);
if(m_hasshadow){
std::string shaName = shadName;
if(shadName=="./res/basicShader.vs"||shadName == "./res/bone.vs"){shaName = shaName.substr(0,shaName.size()-3) + "Shadow.vs";}
m_shadows = new Shader(shaName,"./res/shadow.fs");
}


}
        void draw( const Camera& camera,int coverb = 0);
        void drawshadow( const Camera& camera);
        void bone(Bone* bones);
        glm::vec3 attachbone(glm::vec3 x1,glm::vec3 x2,glm::vec3 x3,glm::vec3 placed);
        virtual ~Actor();

        inline Shader* Shad(){return m_shader;}
   Shader* m_shader;
   Shader* m_shadows;
   Bone* m_bone;
   Mesh* m_mesh;
   Mesh* m_shadow;
   bool m_hasshadow;
   bool* done = new bool(false);
   std::thread worker;
   Transform* m_transform;
   Hitbox* m_hitbox;
   std::vector<Texture*> m_textures;
};

#endif // ACTOR_H
