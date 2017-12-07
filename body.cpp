#include "body.h"

Body::Body(const std::string& meshname,const std::string& hitboxname,const std::string& texname,const std::string& shadername,const std::string& bonename)
{

    m_transform = new Transform();
    m_bones = new Bone(m_transform);
    m_bones->load("./res/"+bonename);
    std::string ff= "";
    for(unsigned int i = 0; i < m_bones->Getbones()->size();i++){

    ff = ff + m_bones->Getbones()[0][i]->name + " ";
     ff = ff + std::to_string(i) + " ";   }
//    std::cout << ff << "\n";
    m_skin = new Actor(meshname,hitboxname,texname,shadername,"./res/basicShader.fs",true,m_transform,ff,m_bones);

 // for(unsigned int z = 0;z<m_bones->Getbones()->size();z++){
 //   m_arrow.push_back( new Actor("./res/arrow.obj","./res/arrow.obj","./res/bricks.jpg","./res/letter",new Transform(),"",false));}
  //m_bones.push_back(new Transform());
}

void Body::draw(Camera camera)
{
m_skin->draw(camera);


}
void Body::drawshadow(Camera camera)
{
m_skin->drawshadow(camera);


}
Body::~Body()
{

    //dtor
}
