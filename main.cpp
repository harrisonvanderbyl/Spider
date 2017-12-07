#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "display.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "transform.h"
#include "camera.h"
#include "hitbox.h"
#include "actor.h"
#include "envelope.h"
#include "envgen.h"
#include "body.h"
#include "wolf.h"
#include "surface.h"
#include <thread>

int main()
{
    Display display(1024,512,"helloworld");

    const Uint8* keys = (SDL_GetKeyboardState(NULL));
    Camera* camera = new Camera(glm::vec3(0.0,0.0,0.0),70.0,2.0/1.0,0.001,1000000.0,keys);
    bool* done = new bool(false);
    std::vector<Actor*> *room = new std::vector<Actor*>;
    //std::thread loading = std::thread(load,room,done);
    //room.push_back(new Actor("./res/cub.obj","./res/cub.obj","./res/bricks.jpg","./res/basicShader"));
    //loading.join();
    room->push_back(new Actor("./res/cub.obj","./res/cub.obj","./res/bricks.jpg","./res/basicShader.vs","./res/skybox.vs",false));
    room->push_back(new Actor("./res/lphouse.obj","./res/lphouse.obj","./res/lphouse.jpg","./res/basicShader.vs","./res/basicShader.fs",true));
    //room->push_back(new Actor("./res/ground.obj","./res/ground.obj","./res/bricks.jpg","./res/basicShader.vs"));
    *(*room)[0]->m_transform->m_scale = glm::vec3(100000.0,-100000.0,100000.0);
    //(*room)[1]->m_transform->m_rot->x = (1.0f);
    // *(*room)[1]->m_transform->m_pos = glm::vec3(100.0f);
   // *(*room)[2]->m_transform->m_pos = glm::vec3(0.0,1.0,0.0);


    Wolf spider(keys);
    room->push_back(spider.m_body->getbody());
    room->push_back(spider.m_legs->getbody());
   // Wolf wolf(keys);

    while(!display.is_closed())

    {     SDL_PumpEvents();





          //CAMERA COLLISION


         display.Clear(1.0,1.0,1.0,1.0);
          (*room)[0]->Shad()->Update(0.0f,"sunlev");
          (*room)[0]->draw(*camera);
          spider.update(camera,(*room)[1]->m_hitbox,*((*room)[1]->m_transform->getMatp()));
          camera->cover = 0;//(int) (*room)[1]->m_hitbox->checkCol(camera->m_position,glm::vec3(0.0,1000.0*glm::sin(camera->time/600.0),1000.0*glm::cos(camera->time/600.0)),*((*room)[1]->m_transform->getMatp()))[0].z;
         // std::cout << camera->cover;
         // room[1]->draw(*camera);
          if(glm::fract((camera->time/600.0)/(2.0*3.1415))<0.5)
          {
              for(int i = 1; i < room->size();i++)
              {
               (*room)[i]->Shad()->Update(1.0f,"sunlev");
               (*room)[i]->draw(*camera);
              }

              for(int i = 1; i < room->size();i++)
              {
               (*room)[i]->drawshadow(*camera);
              }

          }
          for(int i = 1; i < room->size();i++)
          {
           (*room)[i]->Shad()->Update(0.0f,"sunlev");
           (*room)[i]->draw(*camera);
          }

          display.SwapBuffers();
      }



    return 0;
}
