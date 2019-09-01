//
//  mesh.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef mesh_hpp
#define mesh_hpp

#include "../Core/core.h"

#include "../Core/Math/point.h"
#include "../Core/Math/vec.h"
#include "../Core/Math/normal.h"

#include "../Core/Memory/STLMemory.hpp"

#include <memory>

#include <vector>

#include <cstdint>

#include <string_view>

using namespace std;

namespace PRGE
{
    struct Mesh
    {
        inline Mesh(size_t verticesCount, size_t indicesCount) :
            indices(indicesCount),
            vertices(verticesCount),
            normals(verticesCount),
            tangets(verticesCount),
            uvs(verticesCount)
        {}
        
        inline Mesh(Mesh&& mesh) :
            indices{move(mesh.indices)},
            vertices{move(mesh.vertices)},
            normals{move(mesh.normals)},
            tangets{move(mesh.tangets)},
            uvs{move(mesh.uvs)}
        {}

        Mesh(const Mesh&) = delete;

        Mesh& operator = (const Mesh&) = delete;
        Mesh& operator = (Mesh&&) = delete;

        inline size_t verticesCount() const
        {
            return vertices.size();
        }

        inline size_t indicesCount() const
        {
            return indices.size();
        }

        /**
         * Статический метод загружающий модель в виде нескольких мешей.
         * Загрузка текстур в обязанности данного статического метола не входит.
         * 
         * @param path путь до файла
         * @return вектор мешей
        */
        static vector<Mesh> loadModel(const string_view path);

    public:
        vector<uint32_t, STLAllocator<uint32_t>> indices;
        vector<Point3<float>, STLAllocator<Point3<float>>> vertices;
        vector<Normal3f, STLAllocator<Normal3f>> normals;
        vector<Vec3<float>, STLAllocator<Vec3<float>>> tangets;
        vector<Point2<float>, STLAllocator<Point2<float>>> uvs;

        /**
         * TODO: должен присутствовать указатель типа 
         * shared_ptr<Texture<float>, STLAllocator<Texture<float>>> указывающий 
         * альфа маску.
        */

        /**
         * TODO: должен присутствовать статический меотд createMesh который возвращал бы
         * массив Trangle'ов в виде "vector<shared_ptr<IShape>>".
        */
    };
}

#endif /* mesh_hpp */
