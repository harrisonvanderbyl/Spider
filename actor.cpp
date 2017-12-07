#include "actor.h"
#include "mesh.h"

void inito(Hitbox** m_hitbox,Mesh** m_mesha, Mesh** m_shadowa, std::string fileName,std::string hitName, bool hasshadow, std::string ff, bool* done)
{

*m_hitbox = new Hitbox(hitName);
std::cout << "started! \n";
*m_mesha = new Mesh(fileName,ff);
Mesh* m_mesh = *m_mesha;
if(hasshadow){
*m_shadowa = new Mesh(fileName,ff);
Mesh* m_shadow = *m_shadowa;

unsigned int u = m_shadow->getmesh()->indices.size();
for(unsigned int i = 0;i < u;i+=3){
unsigned int ind = m_shadow->getmesh()->indices[i];
m_shadow->getmesh()->normals[ind] = glm::vec3(-1000.0,0.0,0.0);
ind = m_shadow->getmesh()->indices[i+1];
m_shadow->getmesh()->normals[ind] = glm::vec3(0.0,-1000.0,0.0);
ind = m_shadow->getmesh()->indices[i+2];
m_shadow->getmesh()->normals[ind] = glm::vec3(0.0,0.0,-1000.0);
}
//m_shadow->reinit();
}
std::cout << "done! \n";
*done = true;
}

/*
Actor::Actor(const std::string& fileName,const std::string& hitName,const std::string& texName,const std::string& shadName)
{
m_transform = new Transform();
m_mesh = new Mesh(fileName);
m_textures.push_back( new Texture(texName));
m_shader =  new Shader(shadName);
m_hitbox = new Hitbox(hitName);
}*/
Actor::~Actor()
{
    //dtor
}
void Actor::bone(Bone* bones){}




glm::vec3 Actor::attachbone(glm::vec3 x1,glm::vec3 x2,glm::vec3 x3,glm::vec3 placed){
glm::vec3 temp;
glm::vec3 m1 = (x1+x2)/2.0f;
   glm::vec3 m2 = (x3+x2)/2.0f;
   float an1 = glm::acos(glm::dot(normalize(m1-x2),normalize(m2-x2)));
   float an2 = (3.1415f)-an1;
   float bon = glm::distance(m1,m2)/glm::sin(an2);
   glm::vec3 norm = glm::normalize(glm::cross(m2-m1,x2-m1));
   float pdis = glm::dot(placed-m2,norm);
   glm::vec3 place= placed-norm*pdis;
   glm::vec3 point = m1-glm::normalize(glm::cross(norm,x2-m1))*bon*glm::sin(3.1415f/2.0f-glm::acos(glm::dot(glm::normalize(x2-m2),glm::normalize(m1-m2))));
   temp.y =glm::acos(glm::dot(glm::normalize(x2-point),glm::normalize(place-point)));
   temp.x = glm::clamp(
   (( glm::acos(glm::dot(glm::normalize(x1-point),glm::normalize(place-point))))/( glm::acos(glm::dot(glm::normalize(x1-point),glm::normalize(x2-point))))<1.0f)*
   ( glm::acos(glm::dot(glm::normalize(x2-point),glm::normalize(place-point))))/( glm::acos(glm::dot(glm::normalize(x1-point),glm::normalize(x2-point)))),0.0f,1.0f);
   temp.z =glm::clamp(
   ((glm::acos(glm::dot(glm::normalize(x3-point),glm::normalize(place-point))))/(glm::acos(glm::dot(glm::normalize(x3-point),glm::normalize(x2-point))))<1.0f)*
   ( glm::acos(glm::dot(glm::normalize(x2-point),glm::normalize(place-point))))/( glm::acos(glm::dot(glm::normalize(x3-point),glm::normalize(x2-point)))),0.0f,1.0f);
   temp.x = glm::pow(temp.x,2.0f);
   temp.z = glm::pow(temp.z,0.5f);
   temp.y = 1.0f-(temp.x+temp.z);
   return temp;

}
void Actor::drawshadow( const Camera& camera)
{
//glClear(GL_DEPTH_BUFFER_BIT);
if(*done && m_hasshadow){
 if(m_bone != NULL)
 {
 m_bone->attach(m_shadows);
 }
m_shadow->initonce();
glDisable(GL_CULL_FACE);
glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
glDepthMask(GL_FALSE);
glDepthFunc(GL_LESS);
m_shadows->Update(*(m_transform->getMatp()),"world");
m_shadows->Setcam(camera);
m_shadows->Bind();

m_shadows->Update(1.0f,"way");
glStencilFunc(GL_ALWAYS,0,0xFFFFFF);

glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
glStencilOpSeparate(GL_BACK,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
m_shadows->Update(glm::vec3(-1000.0,0.0,0.0),"vert");
m_shadow->draw();
m_shadows->Update(glm::vec3(0.0,-1000.0,0.0),"vert");
m_shadow->draw();
m_shadows->Update(glm::vec3(0.0,0.0,-1000.0),"vert");
m_shadow->draw();

m_shadows->Update(-1.0f,"way");
glStencilOpSeparate(GL_FRONT,GL_KEEP,GL_KEEP,GL_DECR_WRAP);
glStencilOpSeparate(GL_BACK,GL_KEEP,GL_KEEP,GL_INCR_WRAP);
m_shadows->Update(glm::vec3(-1000.0,0.0,0.0),"vert");
m_shadow->draw();
m_shadows->Update(glm::vec3(0.0,-1000.0,0.0),"vert");
m_shadow->draw();
m_shadows->Update(glm::vec3(0.0,0.0,-1000.0),"vert");
m_shadow->draw();
//m_shadow->draw();
glDepthMask(GL_TRUE);}
//glClear(GL_DEPTH_BUFFER_BIT);
}

void Actor::draw( const Camera& camera, int coverb)
{

if(*done){
 if(m_bone != NULL)
 {
 m_bone->attach(m_shader);
 }
int cover = camera.cover * coverb;
m_mesh->initonce();
//glEnable(GL_CULL_FACE);
glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
int ffa = glm::mix(GL_KEEP,GL_REPLACE,(float)(cover!=0));
glStencilOp(ffa,ffa,ffa);
int ffu = glm::mix(GL_EQUAL,GL_ALWAYS,(float)(cover!=0));
glStencilFunc(ffu,cover,0xFFFFFF);
glDepthFunc(GL_LEQUAL);
glDepthMask(GL_TRUE);
m_shader->Update(*(m_transform->getMatp()),"world");
m_shader->Setcam(camera);
m_shader->Bind();

for(unsigned int i = 0; i < m_textures.size();i++){
m_textures[i]->Bind(i);}
m_shader->Update(1,"TEXTURE1");
for(unsigned int i = 0;i < camera.lights.size();i++)
{
m_shader->Update(camera.lights[i],"lamp["+std::to_string(i)+"]");
}
m_mesh->draw();
glDisable(GL_CULL_FACE);
}
}
