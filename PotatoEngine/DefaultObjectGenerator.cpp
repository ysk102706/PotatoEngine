#include "DefaultObjectGenerator.h"
#include <vector>

namespace Engine {
    MeshData DefaultObjectGenerator::MakeTriangle(float scale)
    {
        MeshData meshData;

        std::vector<Vector3> pos;
        std::vector<Vector3> normal;
        std::vector<Vector2> tex;

        pos.push_back(Vector3(0.0f, 0.5f, 0.0f) * scale); 
        pos.push_back(Vector3(0.5f, -0.5f, 0.0f) * scale);
        pos.push_back(Vector3(-0.5f, -0.5f, 0.0f) * scale);
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(0.0f, 1.0f));


        for (int i = 0; i < pos.size(); i++) {
            Vertex v;

            v.position = pos[i];

            meshData.vertices.push_back(v);
        }

        meshData.indices = {
            0, 1, 2
        };

        return meshData;
    }

    MeshData DefaultObjectGenerator::MakeBox(float scale)
    {
        MeshData meshData;

        std::vector<Vector3> pos;
        std::vector<Vector3> normal;
        std::vector<Vector2> tex;

        pos.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        pos.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        normal.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, 1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, 1.0f, 0.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        pos.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        pos.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        normal.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, -1.0f, 0.0f));
        normal.push_back(Vector3(0.0f, -1.0f, 0.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        pos.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        pos.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale); 
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, -1.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        pos.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale);
        pos.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        pos.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        normal.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, 1.0f));
        normal.push_back(Vector3(0.0f, 0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        pos.push_back(Vector3(1.0f, -1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, -1.0f) * scale);
        pos.push_back(Vector3(1.0f, 1.0f, 1.0f) * scale); 
        pos.push_back(Vector3(1.0f, -1.0f, 1.0f) * scale);
        normal.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(1.0f, 0.0f, 0.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        pos.push_back(Vector3(-1.0f, -1.0f, 1.0f) * scale);
        pos.push_back(Vector3(-1.0f, 1.0f, 1.0f) * scale);
        pos.push_back(Vector3(-1.0f, 1.0f, -1.0f) * scale);
        pos.push_back(Vector3(-1.0f, -1.0f, -1.0f) * scale);
        normal.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        normal.push_back(Vector3(-1.0f, 0.0f, 0.0f));
        tex.push_back(Vector2(0.0f, 1.0f));
        tex.push_back(Vector2(0.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 0.0f));
        tex.push_back(Vector2(1.0f, 1.0f));

        for (int i = 0; i < pos.size(); i++) {
            Vertex v;
            
            v.position = pos[i];
            v.normal = normal[i]; 
            v.texcoord = tex[i]; 

            meshData.vertices.push_back(v); 
        }
        
        meshData.indices = {
             0,  1,  2,  0,  2,  3, 
             4,  5,  6,  4,  6,  7, 
             8,  9, 10,  8, 10, 11, 
            12, 13, 14, 12, 14, 15, 
            16, 17, 18, 16, 18, 19, 
            20, 21, 22, 20, 22, 23 
        };

        return meshData;
    }

}