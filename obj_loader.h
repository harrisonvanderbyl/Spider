#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct OBJIndex
{
    unsigned int vertexIndex;
    unsigned int uvIndex;
    unsigned int normalIndex;
   unsigned int boneIndex;
   unsigned int weightIndex;

    bool operator<(const OBJIndex& r) const
    {
        return vertexIndex < r.vertexIndex;
    }
};

class IndexedModel
{
public:
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec4> indexes;
    std::vector<glm::vec4> weights;
    std::vector<glm::vec4> indexes2;
    std::vector<glm::vec4> weights2;
    std::vector<glm::vec4> indexes3;
    std::vector<glm::vec4> weights3;
    std::vector<unsigned int> indices;
    std::vector<float> area;
    void CalcNormals();
};

class OBJModel
{
public:
    std::vector<OBJIndex> OBJIndices;
    std::vector<glm::vec3> vertices;
    std::vector<std::vector<float>> indexes;
    std::vector<std::vector<float>> weights;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    bool hasUVs;
    bool hasNormals;
    bool hasWeights = 0;
    bool hasBones = 0;
    OBJModel(const std::string& fileName,const std::string& ff = "");
    std::string translate;
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
    void CreateOBJFace(const std::string& line);
    std::vector<float> parseweights(const std::string& line1);
    std::vector<float> parsebones(const std::string& line1);
    std::vector<float> parsebonesb(const std::string& line1);

    glm::vec2 ParseOBJVec2(const std::string& line);
    glm::vec3 ParseOBJVec3(const std::string& line);
    OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals, bool* hasBones, bool* hasWeights);
};

#endif // OBJ_LOADER_H_INCLUDED
