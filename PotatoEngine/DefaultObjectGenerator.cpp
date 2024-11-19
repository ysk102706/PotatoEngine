#include "DefaultObjectGenerator.h"
#include <vector>
#include <DirectXMath.h> 
#include "ResourceLoader.h"

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
            v.normal = normal[i]; 
            v.texcoord = tex[i]; 
            v.tangent = Vector3(1.0f, 0.0f, 0.0f); 

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
                v.tangent = Vector3(1.0f, 0.0f, 0.0f); 

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

                float cosY = cos(angleY);
                float sinY = sin(angleY);
                float cosX = cos(angleX);
                float sinX = sin(angleX);

                v.position = Vector3(cosY * radius * cosX, sinX * radius, sinY * radius * cosX); 
                v.normal = Vector3(cosY * cosX, sinX, sinY * cosX); 
                v.texcoord = Vector2(float(j) / column, 1 - float(i) / row); 

                Vector3 up = Vector3(0.0f, 1.0f, 0.0f); 
                
                Vector3 normalOrth = v.normal - up.Dot(v.normal) * v.normal; 
                normalOrth.Normalize(); 

                v.tangent = up.Cross(normalOrth); 
                v.tangent.Normalize(); 

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

    MeshData DefaultObjectGenerator::SubdivideToSphere(float radius, MeshData meshData)
    {
        for (auto& a : meshData.vertices) {
            a.position = a.normal * radius;
        }

        auto MoveVertex = [&](Vertex& v) {
            v.normal = v.position;
            v.normal.Normalize(); 
            v.position = v.normal * radius; 

            Vector3 up = Vector3(0.0f, 1.0f, 0.0f);

            Vector3 normalOrth = v.normal - up.Dot(v.normal) * v.normal;
            normalOrth.Normalize();

            v.tangent = up.Cross(normalOrth);
            v.tangent.Normalize();
        }; 

        MeshData newMeshData;
        int idx = 0;
        for (int i = 0; i < meshData.indices.size(); i += 3) {
            int i0 = meshData.indices[i];
            int i1 = meshData.indices[i + 1];
            int i2 = meshData.indices[i + 2];

            Vertex v0 = meshData.vertices[i0];
            Vertex v1 = meshData.vertices[i1];
            Vertex v2 = meshData.vertices[i2]; 

            Vertex v3;
            v3.position = (v0.position + v1.position) * 0.5f;
            v3.texcoord = (v0.texcoord + v1.texcoord) * 0.5f; 
            MoveVertex(v3);

            Vertex v4;
            v4.position = (v1.position + v2.position) * 0.5f;
            v4.texcoord = (v1.texcoord + v2.texcoord) * 0.5f;
            MoveVertex(v4);

            Vertex v5;
            v5.position = (v2.position + v0.position) * 0.5f;
            v5.texcoord = (v2.texcoord + v0.texcoord) * 0.5f;
            MoveVertex(v5); 

            newMeshData.vertices.push_back(v0);
            newMeshData.vertices.push_back(v3);
            newMeshData.vertices.push_back(v5);
            
            newMeshData.vertices.push_back(v3);
            newMeshData.vertices.push_back(v1);
            newMeshData.vertices.push_back(v4);
            
            newMeshData.vertices.push_back(v4);
            newMeshData.vertices.push_back(v2);
            newMeshData.vertices.push_back(v5);

            newMeshData.vertices.push_back(v3);
            newMeshData.vertices.push_back(v4);
            newMeshData.vertices.push_back(v5);

            for (int j = 0; j < 12; j++) {
                newMeshData.indices.push_back(j + idx);
            } 
            idx += 12;
        } 

        return newMeshData;
    }

    std::vector<MeshData> DefaultObjectGenerator::ReadFromFile(std::string path, std::string filename, bool revertNormal)
    {
        std::vector<MeshData> meshes = ResourceLoader::LoadModel(path, filename, revertNormal); 
        
        Vector3 vmin(1000, 1000, 1000);
        Vector3 vmax(-1000, -1000, -1000);
        for (auto& a : meshes) {
            for (auto& b : a.vertices) {
                vmin.x = XMMin(vmin.x, b.position.x);
                vmin.y = XMMin(vmin.y, b.position.y);
                vmin.z = XMMin(vmin.z, b.position.z);
                vmax.x = XMMax(vmax.x, b.position.x);
                vmax.y = XMMax(vmax.y, b.position.y);
                vmax.z = XMMax(vmax.z, b.position.z);
            }
        }

        float dx = vmax.x - vmin.x, dy = vmax.y - vmin.y, dz = vmax.z - vmin.z;
        float dl = XMMax(XMMax(dx, dy), dz); 
        float cx = (vmax.x + vmin.x) * 0.5f, cy = (vmax.y + vmin.y) * 0.5f, cz = (vmax.z + vmin.z) * 0.5f; 

        for (auto& a : meshes) {
            for (auto& b : a.vertices) {
                b.position.x = (b.position.x - cx) / dl;
                b.position.y = (b.position.y - cy) / dl;
                b.position.z = (b.position.z - cz) / dl;
            }
        }

        return meshes;
    }

    std::vector<Vector4> DefaultObjectGenerator::MakeTessellationQuad()
    {
        std::vector<Vector4> vertices; 

        vertices.push_back(Vector4(-1.0f, 1.0f, 1.0f, 1.0f));
        vertices.push_back(Vector4(1.0f, 1.0f, 1.0f, 1.0f)); 
        vertices.push_back(Vector4(-1.0f, -1.0f, 1.0f, 1.0f)); 
        vertices.push_back(Vector4(1.0f, -1.0f, 1.0f, 1.0f));

        return vertices;
    }

}