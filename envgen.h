#ifndef ENVGEN_H
#define ENVGEN_H
#include "actor.h"
#include "transform.h"
#include <iostream>
#include <string>
#include <fstream>
class Envgen
{
    public:
        Envgen(int* keys) {
        m_keys = keys;
        m_trans = new Transform();
     //   int* fa = new int[12] {0,0,0,0,0,0,0,0,0,0,0,0};
       // m_env = new Envelope(fa,"2",m_trans);
        load();
        //m_env->getMod()->breakLink()=;
        }
        void Draw(const Camera& camera){
        if (m_keys[12]){save();
        m_keys[12] = 0;
        }
        for(unsigned int i = 0; i < m_envelopes.size();i++)
        {

        m_envelopes[i]->Draw(camera);
        }
      //  m_env->Draw(camera);
        }
        inline Transform* getMod(){
        return m_trans;
        }
        void save(){
        std::ofstream out("letters.txt",std::ios::out | std::ios::app | std::ios::binary);

        if(out.is_open()){
        out << m_trans->m_pos->x <<"\n";
        out << m_trans->m_pos->y << "\n";
        out << m_trans->m_pos->z << "\n";
        out << m_trans->m_rot->x << "\n";
        out << m_trans->m_rot->y << "\n";
        out << m_trans->m_rot->z << "\n";
        //out << "\n";
         out.close();
         }
         else{std::cout << "cannot open";}

        m_envelopes.push_back(new Envelope(m_keys,"1",m_trans));
        ((m_envelopes[m_envelopes.size()-1])->getMod()->breakLink());
        }
        void load(){
        std::ifstream in("letters.txt",std::ios::out | std::ios::app | std::ios::binary);
        std::string line,line1,line2;
       if(in.is_open()){
       while(std::getline(in,line)){

      std::getline(in,line1);
      std::getline(in,line2);
       *(m_trans->m_pos)=(glm::vec3(std::stof(line) , std::stof(line1)  ,std::stof(line2)) );
       std::getline(in,line);
        std::getline(in,line1);
      std::getline(in,line2);
        *(m_trans->m_rot)=(glm::vec3(std::stof(line) , std::stof(line1)  ,std::stof(line2)) );
       *(m_trans->m_scale)=(glm::vec3(1.0));

       std::string te;
       te.push_back(char(48+num++));
        m_envelopes.push_back(new Envelope(m_keys,te,m_trans));
        ((m_envelopes[m_envelopes.size()-1])->getMod()->breakLink());

        line = "";
         }
        in.close();
       }
       else{

       std::cout << "cannot read file;";
       }


        }
    protected:
    private:
    int* m_keys;
    Envelope* m_env;
    std::vector<Envelope*> m_envelopes;
    Transform* m_trans;
    Transform* temp = new Transform;
    int num = 1;
};

#endif // ENVGEN_H
