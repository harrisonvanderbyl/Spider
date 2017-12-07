#include "wolf.h"

Wolf::Wolf(const Uint8* keys)
{
    m_body = new Body("./res/spiderlegs.obj","./res/spiderlegs.obj","./res/spider.jpg","./res/bone.vs","spider.bvh");
    m_legs = new Body("./res/spiderbody.obj","./res/spiderbody.obj","./res/spider.jpg","./res/bone.vs","spider.bvh");
    m_legs->m_bones = m_body->m_bones;
    m_body->m_bones->Getbones()[0][0]->m_linka = m_trans;
    m_legs->getbody()->m_bone = m_body->m_bones;
   m_keys = keys;
   poss = glm::vec3(0.0,100.0,0.0);
}
 void Wolf::draw(Camera* camera)
 {

  m_body->draw(*camera);
  m_legs->draw(*camera);



 }
 void Wolf::drawshadow(Camera* camera){
    m_body->drawshadow(*camera);
    m_legs->drawshadow(*camera);

    }
 void Wolf::update(Camera* camera,Hitbox* hit, glm::mat4 check){
  if(lightindex == -1){
  lightindex = camera->lights.size();
  camera->lights.push_back(glm::vec4(poss,level));
  }
  camera->lights[lightindex]=(glm::vec4(poss,16.0*level*level));
  action = mem;

 //camera->time =  float(m_rotor.m_val)*30.0f;
  moss = glm::vec3(0.0);
  //glm::mat4 rr = *m_body->m_bones->Getbones()[0][0]->m_linka* *m_body->m_bones->Getbones()[0][0]->m_mymodel;//glm::inverse(glm::lookAt(glm::vec3(0.0),norm,glm::normalize(glm::cross(norm,glm::vec3(0.0,0.0,1.0)))))*glm::rotate(-dir,glm::vec3(0.0,1.0,0.0));
  glm::vec3 sw = glm::normalize(glm::cross(normto,camera->m_forward));
  glm::vec3 fw = glm::normalize(glm::cross(normto,sw));
  if(m_keys[SDL_SCANCODE_0]){action = 0;}
  if(m_keys[SDL_SCANCODE_MINUS]){level*=1.1f;}
  if(m_keys[SDL_SCANCODE_EQUALS]){level*=0.9f;}
  if(m_keys[SDL_SCANCODE_W]&&!m_keys[SDL_SCANCODE_LSHIFT]){
  action = 1;
  moss -= fw*level;
  }
  if(m_keys[SDL_SCANCODE_S]&&!m_keys[SDL_SCANCODE_LSHIFT]){
  action = 2;
  moss += fw*level;
  }
  if(m_keys[SDL_SCANCODE_A]&&!m_keys[SDL_SCANCODE_LSHIFT]){
  action = 3;
  moss += sw*level;
  }
  if(m_keys[SDL_SCANCODE_D]&&!m_keys[SDL_SCANCODE_LSHIFT]){
  action = 4;
  moss -= sw*level;
  }
  if(m_keys[SDL_SCANCODE_W]&&m_keys[SDL_SCANCODE_LSHIFT]){
  action = 5;
  moss -= 2.0f*fw*level;
  }
  if(m_keys[SDL_SCANCODE_S]&&m_keys[SDL_SCANCODE_LSHIFT]){
  action = 6;
  moss += 2.0f*fw*level;
  }
  if(m_keys[SDL_SCANCODE_A]&&m_keys[SDL_SCANCODE_LSHIFT]){
  action = 7;
  moss += 2.0f*sw*level;
  }
  if(m_keys[SDL_SCANCODE_D]&&m_keys[SDL_SCANCODE_LSHIFT]){
  action = 8;
  moss -= 2.0f*sw*level;
  }

  std::vector<glm::vec3> che;
  che = hit->checkCol(poss,poss-(velocity+normto)*level*3.0f,check);
  bool isfalling = che[0].y > 1000 * level;
  norm = glm::mix(norm,normto,0.05);
  if(isfalling){

  normto = glm::normalize(glm::mix(normto,glm::vec3(0.0,1.0,0.0),che[0].x*0.05));

  if ((camera->time)-anitime > (float)m_body->m_bones->m_animate[9].size())
  {action = 10;
  mem = 10;
  }


  velocity += glm::vec3(0.0f,((9.8f)/30.0f),0.0f)*level;
  }else{
  mem = 0;
 velocity = glm::vec3(0.0);
  normto = che[2];
 }
  if(m_keys[SDL_SCANCODE_SPACE]){

  velocity = -normto*5.0f*level;
  if (mem == 0) {
  anitime = (camera->time)-10.0f;
  action = 9;
  mem = 9;}
  }

  moss -= velocity;







  che = hit->checkCol(poss,poss+(moss),check);
  poss = che[3];
  if(che[0].x < 0.5){
    normto = che[2];}
  // poss = poss+moss;//hit->checkCol(poss,foot+moss+mov,check)[1];
 // poss = glm::mix(poss,foot-norm*level*0.3f,0.1);

*m_trans = glm::inverse(glm::lookAt(glm::vec3(0.0),glm::normalize(glm::cross(norm,glm::cross(norm,camera->m_forward))),norm))*glm::rotate(-3.1415f/2.0f,glm::vec3(1.0,0.0,0.0));//glm::normalize(glm::cross(norm,glm::vec3(0.0,0.0,1.0)))))*glm::rotate(3.1415f,glm::vec3(1.0,0.0,0.0))*glm::rotate(dir-3.1415f/2.0f,glm::vec3(0.0,0.0,1.0));
 m_body->m_bones->animate((camera->time)-anitime,action,3.0);

*m_body->m_bones->Getbones()[0][0]->m_pos = poss+normto*level*2.0f;
*m_body->m_bones->Getbones()[0][0]->m_scale = glm::vec3(level);


m_legs->m_transform->update();
m_body->m_transform->update();
camera->update();
(camera->m_position) =  poss - level*30.0f*camera->m_forward;//hit->checkCol(poss+norm*level*1.1f, poss - level*30.0f*camera->m_forward,check)[3];
//camera->m_perspective = glm::ortho(-10.0f*level,10.0f*level,-10.0f*level,10.0f*level,camera->zNear,camera->zFar) ;
camera->m_up = norm;
dir = glm::mix(dir,-camera->m_aangle,0.1);
//m_body->m_bones->Getbones()[0][5]->m_rot->y += float(m_rotor.m_val)/3.0f;
//(camera->m_forward) = glm::vec3(glm::sin(dir),-1.0,glm::cos(dir));
//camera->time += 1.0/600.0;





    }
