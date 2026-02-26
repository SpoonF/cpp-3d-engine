#include "model.h"

#include <fstream>
#include <string.h>

Model::Model(const char* filename) {
    FILE* file = fopen(filename, "r");

    assert(file != NULL && "Impossible to open the file !\n");

    std::vector<glm::vec3> tmp_vertices;
    std::vector<glm::vec2> tmp_uvs;
    std::vector<glm::vec3> tmp_normals;
    std::vector<int> fragVert;
    std::vector<int> fragUV;
    std::vector<int> fragNorm;

    while (true)
    {
        char lineHeader[128];

        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF) break;

        if ( strcmp(lineHeader, "v") == 0 ) {
            glm::vec3 vertex; 
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            tmp_vertices.push_back(vertex);
        } else if ( strcmp(lineHeader, "vt") == 0 ) {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            tmp_uvs.push_back(uv);
        } else if ( strcmp(lineHeader, "vn") == 0 ) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            tmp_normals.push_back(normal);
        } else if ( strcmp(lineHeader, "f") == 0 ) {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            assert(matches == 9 && "Fuck!");

            
            fragVert.push_back(vertexIndex[0]);
            fragVert.push_back(vertexIndex[1]);
            fragVert.push_back(vertexIndex[2]);
            fragUV.push_back(uvIndex[0]);
            fragUV.push_back(uvIndex[1]);
            fragUV.push_back(uvIndex[2]);
            fragNorm.push_back(normalIndex[0]);
            fragNorm.push_back(normalIndex[1]);
            fragNorm.push_back(normalIndex[2]);
        }
    }

    for (size_t i = 0; i < fragVert.size(); i++)
    {
        vertices.push_back(tmp_vertices[ fragVert[i] - 1 ]);
    }

    for (size_t i = 0; i < fragUV.size(); i++)
    {
        uvs.push_back(tmp_uvs[ fragUV[i] - 1 ]);
    }

    for (size_t i = 0; i < fragNorm.size(); i++)
    {
        normals.push_back(tmp_normals[ fragNorm[i] - 1 ]);
    }
}