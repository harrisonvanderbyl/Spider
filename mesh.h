#ifndef MESH_H
#define MESH_H
#include <glm/glm.hpp>
#include <GL/glew.h>
//#include "bone.h"
#include "obj_loader.h"

class Vertex
{
public:
    Vertex(const glm::vec3& pos,const glm::vec2& texCoord,const glm::vec3& normal = glm::vec3(0.0,0.0,0.0),const glm::vec4& index = glm::vec4(0),const glm::vec4& weights = glm::vec4(1.0,0.0,0.0,0.0),const glm::vec4& ind2 = glm::vec4(0),const glm::vec4& w2 = glm::vec4(0.0,0.0,0.0,0.0),const glm::vec4& ind3 = glm::vec4(0),const glm::vec4& w3 = glm::vec4(0.0,0.0,0.0,0.0) )
    {
        this->pos = pos;
        this->texCoord = texCoord;
        this->normal = normal;
        this->index = index;
        this->weights = weights;
          this->index2 =ind2;
        this->weights2 = w2;
          this->index3 = ind3;
        this->weights3 = w3;


    }
    inline glm::vec3* getpos()
    {
        return &pos;
    };
    inline glm::vec2* gettexCoord()
    {
        return &texCoord;
    };
       inline glm::vec4* getindex()
    {
        return &index;
    };
           inline glm::vec4* getweights()
    {
        return &weights;
    };
         inline glm::vec4* getindex2()
    {
        return &index2;
    };
           inline glm::vec4* getweights2()
    {
        return &weights2;
    };
         inline glm::vec4* getindex3()
    {
        return &index3;
    };
           inline glm::vec4* getweights3()
    {
        return &weights3;
    };

    inline glm::vec3* getnormal()
    {
        return &normal;
    };

protected:
private:
    glm::vec3 pos;
    glm::vec2 texCoord;
    glm::vec3 normal;
    glm::vec4 index;
    glm::vec4 weights;
     glm::vec4 index2;
    glm::vec4 weights2;
     glm::vec4 index3;
    glm::vec4 weights3;
};

class Mesh
{
public:
    Mesh(Vertex* verticies,unsigned int numVerticies, unsigned int* indices,unsigned int numindices);
    Mesh(const std::string& fileName,const std::string& ff = "");
    inline IndexedModel* getmesh(){
    return m_model;

    }
    void reinit(){InitMesh(*m_model);}
    void initonce(){
    if(!isinit){
    InitMesh(*m_model);
    isinit = true;
    }
    }
    void draw();
    virtual ~Mesh();
    bool isinit = false;
 /*   Mesh(const Mesh& other)
    {
   *this = &other;
    };
    Mesh& operator=(const Mesh& other)
    {
    return other;
    };*/
protected:

private:



    void InitMesh(const IndexedModel& model);

    enum
    {
        POSITION_VB,
        TEXCOORD_VB,
        NORMAL_VB,
        BONES_VB,
        WEIGHT_VB,
        BONES2_VB,
        WEIGHT2_VB,
        BONES3_VB,
        WEIGHT3_VB,
        INDEX_VB,
        NUM_BUFFERS
    };

    GLuint m_vertexArrayObject;
    GLuint m_vertexArrayBuffers[NUM_BUFFERS];
    IndexedModel* m_model;
    unsigned int a_drawCount;

};

#endif // MESH_H
