#include "mesh.h"
#include <vector>
#include "obj_loader.h"
#include <iostream>
Mesh::Mesh(const std::string& fileName,const std::string& ff)
{
     m_model = new IndexedModel();
     OBJModel temp(fileName,ff);

//     std::cout << ff;
    *m_model = temp.ToIndexedModel();

    //InitMesh(*m_model);
}

Mesh::Mesh(Vertex* verticies,unsigned int numVerticies,unsigned int* indices,unsigned int numindices)
{
    m_model = new IndexedModel();

    for(unsigned int i = 0; i < numVerticies; i++)
    {
        m_model->positions.push_back(*verticies[i].getpos());
        m_model->texCoords.push_back(*verticies[i].gettexCoord());
        m_model->normals.push_back(*verticies[i].getnormal());
        m_model->indexes.push_back(*verticies[i].getindex());
        m_model->weights.push_back(*verticies[i].getweights());
        m_model->indexes2.push_back(*verticies[i].getindex2());
        m_model->weights2.push_back(*verticies[i].getweights2());
          m_model->indexes3.push_back(*verticies[i].getindex3());
        m_model->weights3.push_back(*verticies[i].getweights3());
    }
    for(unsigned int i = 0; i < numindices; i++)
    {
        m_model->indices.push_back(indices[i]);

    }
    InitMesh(*m_model);

}


Mesh::~Mesh()
{
    glDeleteVertexArrays(1,&m_vertexArrayObject);
}

void Mesh::InitMesh(const IndexedModel& model)
{
    a_drawCount = model.indices.size();

    glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);

    glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[POSITION_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.positions.size() * sizeof(model.positions[0]), &model.positions[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,0,0);


    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[TEXCOORD_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.texCoords.size() * sizeof(model.texCoords[0]), &model.texCoords[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,2,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[NORMAL_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.normals.size() * sizeof(model.normals[0]),&model.normals[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,3,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[BONES_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.indexes.size() * sizeof(model.indexes[0]),&model.indexes[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3,4,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[WEIGHT_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.weights.size() * sizeof(model.weights[0]),&model.weights[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4,4,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[BONES2_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.indexes.size() * sizeof(model.indexes2[0]),&model.indexes2[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5,4,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[WEIGHT2_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.weights.size() * sizeof(model.weights2[0]),&model.weights2[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6,4,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[BONES3_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.indexes.size() * sizeof(model.indexes3[0]),&model.indexes3[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7,4,GL_FLOAT, GL_FALSE,0,0);

    glBindBuffer(GL_ARRAY_BUFFER,m_vertexArrayBuffers[WEIGHT3_VB]);
    glBufferData(GL_ARRAY_BUFFER,model.weights.size() * sizeof(model.weights3[0]),&model.weights3[0],GL_STATIC_DRAW);
    glEnableVertexAttribArray(8);
    glVertexAttribPointer(8,4,GL_FLOAT, GL_FALSE,0,0);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_vertexArrayBuffers[INDEX_VB]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,model.indices.size() * sizeof(model.indices[0]),&model.indices[0],GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::draw()
{

    glBindVertexArray(m_vertexArrayObject);
    glDrawElements(GL_TRIANGLES,a_drawCount,GL_UNSIGNED_INT,0);
    // glDrawArrays(GL_TRIANGLES,0,a_drawCount);
    glBindVertexArray(0);

}
