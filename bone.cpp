#include "bone.h"
#include <iostream>
Bone::Bone(Transform* t)
{
    m_t = t;
    m_diagnosticbone = 0;
}

void Bone::attach(Shader* shad)
{

    for(unsigned int i = 0; i < m_bones.size(); i++)
    {

        shad->Update(*(m_bones)[i]->getMatbone(),"bone["+std::to_string(i)+"]");

    }
}


Bone::~Bone()
{
    //dtor
}
std::vector<float> Bone::parsebones(const std::string& line1)
{
//std::string line = line1.substr(2,line.npos-2);
    std::vector<float> ret;
    unsigned int endtok = 0;
    std::string line = line1;
    for(unsigned int starttok = line.find(" "); line.find_first_not_of(" ") != line.npos; starttok = endtok) //line.find(" ",starttok+1)){
    {

        int endtok = line.find(" ",starttok+1);
        std::string line2 = line.substr(starttok,endtok);
        line.erase(starttok,endtok);

        if (line2!=" ")
        {
            ret.push_back(std::stof(line2));
        }
//std::cout << (line2) << "\n";
    }





    return ret;
}

void Bone::save() {        }

void Bone::load(std::string fname)
{
    std::vector<Transform*> be;
    std::vector<glm::vec3> bep;
    std::vector<int> ic;
    std::ifstream in(fname,std::ios::out | std::ios::app | std::ios::binary);
    std::string line,line1,line2,line3;
    int c;
    //    std::cout<<"a";
    if(in.is_open())
    {
        // std::cout<<"b";
        c = -1;
        // int jj = 0;
        while( !in.eof() )
        {

            std::getline(in,line);

// std::cout<<"c";
            if ((line.find("ROOT")!=line.npos) || (line.find("JOINT")!=line.npos) || (line.find("End")!=line.npos))
            {
                std::string rea = line;
                std::getline(in,line);
                std::getline(in,line);
                unsigned int x = line.find(".");
                if (x != line.npos)
                {

                    line = line.substr(x-2);
                    line = " " + line;


                    std::vector<float> poss = parsebones(line);
                    std::getline(in,line);

                    glm::vec3 pos = (glm::vec3(poss[0], poss[2]  ,-poss[1]));


                    if(rea.find("End")==rea.npos)
                    {

                        chain.push_back(c);


                        if ((rea.find("ROOT")==rea.npos) )
                        {
                            m_bones.push_back(new Transform(glm::vec3(0.0),glm::vec3(0.0),glm::vec3(1.0f/1.0f),(m_bones[c])->getMatp(),pos,m_bones[c]->getb(),new glm::vec3(pos+*m_bones[c]->getb()),(m_bones[c])->m_bonmat));
                            rea = rea.substr(rea.find("T")+1);
                            while(rea[0]==' ')
                            {
                                rea = rea.substr(1);
                            }
                            m_bones[m_bones.size()-1]->name = rea;//rea.substr(rea.find_first_not_of("JOINT "));
                            m_bonepos.push_back(pos+m_bonepos[c]);
                            m_bones[c]->setlf(m_bones[m_bones.size()-1]->getb()) ;
                        }
                        else
                        {
                            // pos += glm::vec3(0.0,0.0,2.0);
                            m_bones.push_back(new Transform(glm::vec3(0.0,0.0,0.0),glm::vec3(0.0),glm::vec3(1.0),m_t->getMatp(),pos,new glm::vec3(pos),new glm::vec3(glm::vec3(pos))));
                            m_bonepos.push_back(glm::vec3(pos));

                        }

                        c = chain.size() - 1;
                        //int jj = 1;
                    }
                    else
                    {
                        bep.push_back(pos+m_bonepos[c]);
                        be.push_back(new Transform(glm::vec3(0.0),glm::vec3(0.0),glm::vec3(1.0),(m_bones[c])->getMatp(),pos,m_bones[c]->getb(),new glm::vec3(pos+*m_bones[c]->getb()),(m_bones[c])->m_bonmat));
                        m_bones[c]->setlf(  be[be.size()-1]->getb());
                        ic.push_back(c);
                        //c = chain[c];
                        //  c = chain.size() - 1;
                    }
                }
            }
            else
            {

                if(line.find("}")!=line.npos)
                {

                    c = chain[c];

                }
            }

            if(line.find("MOTION")!=line.npos)
            {
                std::vector<std::vector<float>> temp;
                std::getline(in,line);

                line = line.substr(7);
                int re = stoi(line);
                std::getline(in,line);
                line = line.substr(11);
                m_frametime = stof(line);
                for(int i = 0; i < re; i++)
                {
                    std::getline(in,line);

                    temp.push_back(parsebones(" "+line));

                }
                m_animate.push_back(temp);
            }

            if (line.find("Xposition")!=line.npos)
            {
                m_channels.push_back(m_bones[chain.size()-1]->m_pos);
                m_PosRot.push_back(false);
            }
            if (line.find("Xrotation")!=line.npos)
            {
                m_channels.push_back(m_bones[chain.size()-1]->m_rot);
                 m_PosRot.push_back(true);
            }



            //glm::vec3 rot =(glm::vec3(std::stof(line) , std::stof(line1)  ,std::stof(line2)) );



        }
        in.close();
    }
    else
    {

        std::cout << "cannot read bone file;";
    }

    for(unsigned int a = 0; a < be.size(); a++)
    {
        //   std::cout << bep[a].x;
        m_bonepos.push_back(bep[a]);
        m_bones.push_back(be[a]);
        chain.push_back(ic[a]);
    }

}
