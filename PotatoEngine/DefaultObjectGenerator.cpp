#include "DefaultObjectGenerator.h"
#include <vector>
#include <DirectXMath.h> 

namespace Engine {
    using namespace DirectX;

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

    MeshData DefaultObjectGenerator::MakeSquareGrid(float scale, int column, int row)
    {
        MeshData meshData;

        float dx = 2.0f / column;
        float dy = 2.0f / row;

        for (int i = 0; i <= row; i++) {
            for (int j = 0; j <= column; j++) {
                Vertex v;

                v.position = Vector3(-1 + j * dx, -1 + i * dy, 0.0f) * scale;
                v.normal = Vector3(0.0f, 0.0f, -1.0f);
                v.texcoord = Vector2(float(j) / column, 1 - float(i) / row); 

                meshData.vertices.push_back(v);
            }
        } 

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);

                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);
                meshData.indices.push_back((column + 1) * i + j + 1);
            }
        }

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
    MeshData DefaultObjectGenerator::MakeCylinder(float topRadius, float bottomRadius, float height, int column, int row)
    {
        MeshData meshData; 
        
        float theta = XM_2PI / column;
        
        float dx = (topRadius - bottomRadius) / row;
        float dy = height / row; 
        
        for (int i = 0; i <= row; i++) {
            float angle = 0;
            for (int j = 0; j <= column; j++) {
                Vertex v;

                v.position = Vector3(cos(angle) * bottomRadius, height * -0.5f + i * dy, sin(angle) * bottomRadius);
                v.normal = Vector3(cos(angle), 0.0f, sin(angle));
                v.texcoord = Vector2(float(j) / column, 1 - float(i) / row);

                meshData.vertices.push_back(v);

                angle += theta; 
            }
            bottomRadius += dx;
        }

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);

                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);
                meshData.indices.push_back((column + 1) * i + j + 1);
            }
        }

        return meshData;
    }

    MeshData DefaultObjectGenerator::MakeSphere(float radius, int column, int row)
    {
        MeshData meshData;

        float thetaY = XM_2PI / column;
        float thetaX = XM_PI / row;

        float dy = radius * 2 / row;

        float angleX = -XM_PI / 2;
        for (int i = 0; i <= row; i++) {
            float angleY = 0;
            for (int j = 0; j <= column; j++) {
                Vertex v;

                v.position = Vector3(cos(angleY) * radius * cos(angleX), sin(angleX) * radius, sin(angleY) * radius * cos(angleX));
                v.normal = Vector3(cos(angleY) * cos(angleX), sin(angleX), sin(angleY) * cos(angleX)); 
                v.texcoord = Vector2(float(j) / column, 1 - float(i) / row);

                meshData.vertices.push_back(v);

                angleY += thetaY;
            }
            angleX += thetaX;
        }

        for (int i = 0; i < row; i++) {
            for (int j = 0; j < column; j++) {
                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);

                meshData.indices.push_back((column + 1) * i + j);
                meshData.indices.push_back((column + 1) * (i + 1) + j + 1);
                meshData.indices.push_back((column + 1) * i + j + 1);
            }
        }

        return meshData;
    }

}