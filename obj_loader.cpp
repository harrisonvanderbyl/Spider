#include "obj_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

OBJModel::OBJModel(const std::string& fileName,const std::string& ff)
{
    translate = ff;
    hasUVs = true;
    hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);

            unsigned int lineLength = line.length();

            if(lineLength < 2)
                continue;

            const char* lineCStr = line.c_str();

            switch(lineCStr[0])
            {
            case 'v':
                if(lineCStr[1] == 't'){
                    this->uvs.push_back(ParseOBJVec2(line));

                    //hasUVs = true;
                    }
                else if(lineCStr[1] == 'n')
                    this->normals.push_back(ParseOBJVec3(line));
                else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                    this->vertices.push_back(ParseOBJVec3(line));
                else if(lineCStr[1] == 'b'){
                   std::vector<float> ret;
                   ret = parsebonesb(line);


                   for(int y = ret.size();y < 12;y++){
                   ret.push_back(0.0);
                    }
                    this->indexes.push_back(ret);
                //    hasBones = true;
                //     hasWeights = true;
                    }
                else if(lineCStr[1] == 'w'){
                    std::vector<float> ret;
                   ret = parsebones(line);

                    for(int y = ret.size();y < 12;y++){
                   ret.push_back(0.0);

                    }
                     this->weights.push_back(ret);
                    }
                break;
            case 'f':
                CreateOBJFace(line);
                break;
            default:
                break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalcNormals()
{
    for(unsigned int i = 0; i < indices.size(); i += 3)
    {
        int i0 = indices[i];
        int i1 = indices[i + 1];
        int i2 = indices[i + 2];

        glm::vec3 v1 = positions[i1] - positions[i0];
        glm::vec3 v2 = positions[i2] - positions[i0];
        glm::vec3 a = glm::cross(v1, v2);
        glm::vec3 normal = glm::normalize(a);
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;


    }

    for(unsigned int i = 0; i < positions.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}

IndexedModel OBJModel::ToIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;

    unsigned int numIndices = OBJIndices.size();

    std::vector<OBJIndex*> indexLookup;

    for(unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&OBJIndices[i]);

    std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);

    std::map<OBJIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;

    for(unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJIndices[i];

        glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
        glm::vec2 currentTexCoord = uvs[currentIndex->uvIndex];
        glm::vec3 currentNormal;
        std::vector<float> currentindexes;
        std::vector<float> currentweights;
     /*   if(hasUVs){
            currentTexCoord = uvs[currentIndex->uvIndex];}
        else{
            currentTexCoord = glm::vec2(0,0);}*/

        if(hasNormals)
            currentNormal = normals[currentIndex->normalIndex];
        else
            currentNormal = glm::vec3(0,0,0);
        if(hasWeights){
         currentweights = weights[currentIndex->weightIndex];
        }
        else{
        currentweights =  {1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0};

        }
        if(hasBones){

         currentindexes = indexes[currentIndex->boneIndex];
        }
        else{
        currentindexes =  {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

        }
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;

        //Create model to properly generate normals on
        std::map<OBJIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.positions.size();

            normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.positions.push_back(currentPosition);
            normalModel.texCoords.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
            normalModel.indexes.push_back(glm::vec4(currentindexes[0],currentindexes[1],currentindexes[2],currentindexes[3]));
            normalModel.weights.push_back(glm::vec4(currentweights[0],currentweights[1],currentweights[2],currentweights[3]));
            normalModel.indexes2.push_back(glm::vec4(currentindexes[4],currentindexes[5],currentindexes[6],currentindexes[7]));
            normalModel.weights2.push_back(glm::vec4(currentweights[4],currentweights[5],currentweights[6],currentweights[7]));
            normalModel.indexes3.push_back(glm::vec4(currentindexes[8],currentindexes[9],currentindexes[10],currentindexes[11]));
            normalModel.weights3.push_back(glm::vec4(currentweights[8],currentweights[9],currentweights[10],currentweights[11]));
        }
        else
            normalModelIndex = it->second;

        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);

        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.positions.size();
            result.positions.push_back(currentPosition);
            result.texCoords.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
            result.indexes.push_back(glm::vec4(currentindexes[0],currentindexes[1],currentindexes[2],currentindexes[3]));
            result.weights.push_back(glm::vec4(currentweights[0],currentweights[1],currentweights[2],currentweights[3]));
              result.indexes2.push_back(glm::vec4(currentindexes[4],currentindexes[5],currentindexes[6],currentindexes[7]));
            result.weights2.push_back(glm::vec4(currentweights[4],currentweights[5],currentweights[6],currentweights[7]));
              result.indexes3.push_back(glm::vec4(currentindexes[8],currentindexes[9],currentindexes[10],currentindexes[11]));
            result.weights3.push_back(glm::vec4(currentweights[8],currentweights[9],currentweights[10],currentweights[11]));

        }
        else
            resultModelIndex = previousVertexLocation;

        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }

    if(!hasNormals)
    {
        normalModel.CalcNormals();

        for(unsigned int i = 0; i < result.positions.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }

    return result;
};

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
    unsigned int start = 0;
    unsigned int end = indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;

    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];

        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            unsigned int countStart = current;

            for(unsigned int i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];

                if(possibleIndex == currentIndex)
                    continue;

                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;

                countStart--;
            }

            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];

                if(possibleIndex == currentIndex)
                    continue;

                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
                        && (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    std::vector<float> currentbones;
                    std::vector<float> currentweights;

                    if(hasUVs){
                        currentTexCoord = uvs[currentIndex->uvIndex];
                   }
                    else{
                        currentTexCoord = glm::vec2(0,0);}

                    if(hasNormals)
                        currentNormal = normals[currentIndex->normalIndex];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    if(hasBones)
                        currentbones = indexes[currentIndex->boneIndex];
                    else
                        currentbones = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
                    if(hasWeights)
                        currentweights = weights[currentIndex->weightIndex];
                    else
                        currentweights = {1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};

                    for(unsigned int j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPosition == result.positions[j]
                                && ((!hasUVs || currentTexCoord == result.texCoords[j])
                                    && (!hasNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }

            return -1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }

        previous = current;
        current = (end - start) / 2 + start;
    }

    return -1;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));
    this->OBJIndices.push_back(ParseOBJIndex(tokens[2], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));;
    this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));;

    if((int)tokens.size() > 4)
    {
        this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));;
        this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));;
        this->OBJIndices.push_back(ParseOBJIndex(tokens[4], &this->hasUVs, &this->hasNormals, &this->hasBones, &this->hasWeights));;
    }
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals, bool* hasBones, bool* hasWeights)
{
    unsigned int tokenLength = token.length();
    const char* tokenString = token.c_str();

    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

    OBJIndex result;
    result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    result.weightIndex = 0;// ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.boneIndex =  0;//ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    if(vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

    result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;


    if(vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);

    *hasNormals = true;

    if(vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    result.weightIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasWeights = true;

    if(vertIndexEnd >= tokenLength)
        return result;

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    result.boneIndex =  ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasBones = true;
    return result;
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();

    unsigned int vertIndexStart = 2;

    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }

    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}
/*std::vector<float> OBJModel::parseweights(const std::string& line1){
std::string line = line1;
std::vector<float> ret;
for(int starttok = line.find(".")-2;starttok != line.npos;starttok = line.find(".")-2){
std::string line2 = line.substr(starttok,7);
line = line.substr(starttok+7,line.npos-(starttok+7));
ret.push_back(std::stof(line2));
}
for(int y = ret.size()-1;y < 9;y++){
ret.push_back(0.0);
}
return ret;
}*/

std::vector<float> OBJModel::parsebones(const std::string& line1){
std::string line = line1.substr(2,line1.npos-2);

std::vector<float> ret;
unsigned int endtok = 0;

for(unsigned int starttok = line.find(" ");line.find_first_not_of(" ") != line.npos;starttok = endtok){//line.find(" ",starttok+1)){

int endtok = line.find(" ",starttok+1);
std::string line2 = line.substr(starttok,endtok);
line.erase(starttok,endtok);

if (line2!=" "){
ret.push_back(std::stof(line2));}

}



return ret;
}
std::vector<float> OBJModel::parsebonesb (const std::string& line1){
std::string line3 = "";
std::string line = line1.substr(2,line1.npos-2);;
unsigned int endtok = 0;

for(unsigned int starttok = line.find(" ");line.find_first_not_of(" ") != line.npos;starttok = endtok){//line.find(" ",starttok+1)){

int endtok = line.find(" ",starttok+1);
std::string line2 = line.substr(starttok,endtok);
line.erase(starttok,endtok);

if (line2!=" "){

//std::cout << " \n line2: " << line2 ;
//std::cout << " \n line: " << line;
if (line2[line2.length()-1]!=' '){line2 = line2 + " ";}
int x = translate.find(line2);
//std::cout << x;
if(x != -1){
std::string temp = translate.substr(x);
//std::cout << temp << "\n";
if(temp[0] == ' '){temp = temp.substr(1);}
//std::cout <<"\n linefound: " <<temp ;
temp = temp.substr(temp.find(" "));

if(temp[0] == ' '){temp = temp.substr(1);}

temp = temp.substr(0,temp.find(" "));
//std::cout <<"\n output: " <<temp ;

line3 = line3 + temp + " ";}
else
{

line3 = line3 + "0 ";


}
}

}



line3 = "vb "+line3;

return parsebones(line3);
}


glm::vec2 OBJModel::ParseOBJVec2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();

    unsigned int vertIndexStart = 3;

    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }

    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

    return glm::vec2(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }

    return result;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;

    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;

    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }

        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }

    return elems;
}
