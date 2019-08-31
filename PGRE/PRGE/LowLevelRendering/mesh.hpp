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
#include "../Core/Memory/memoryArena.hpp"

#include <memory>

#include <vector>

#include <cstdint>

using namespace std;

namespace PRGE
{
    struct Mesh
    {
        inline Mesh(size_t verticesCount, size_t indicesCount) :
            indices(indicesCount),
            ptrVertices{new Point3<float> [verticesCount]},
            ptrNormals{new Normal3f [verticesCount]},
            ptrTangets{new Vec3<float> [verticesCount]},
            uv{new Point2<float> [verticesCount]},
            nVertices{verticesCount}
        {}

        inline Mesh(size_t verticesCount, size_t indicesCount, MemoryArena& memoryArena) :
            indices(indicesCount),
            ptrVertices{memoryArena.allocator<Point3<float>>(verticesCount)},
            ptrNormals{memoryArena.allocator<Normal3f>(verticesCount)},
            ptrTangets{memoryArena.allocator<Vec3<float>>(verticesCount)},
            uv{memoryArena.allocator<Point2<float>>(verticesCount)},
            nVertices{verticesCount}
        {}
        
        inline Mesh(Mesh&& mesh) :
            indices{move(mesh.indices)},
            ptrVertices{move(mesh.ptrVertices)},
            ptrNormals{move(mesh.ptrNormals)},
            ptrTangets{move(mesh.ptrTangets)},
            uv{move(mesh.uv)},
            nVertices{mesh.nVertices}
        {}

        Mesh(const Mesh&) = delete;

        Mesh& operator = (const Mesh&) = delete;
        Mesh& operator = (Mesh&&) = delete;

        inline size_t verticesCount() const
        {
            return indices.size();
        }

        inline size_t indicesCount() const
        {
            return indices.size();
        }

    public:
        vector<uint32_t, STLAllocator<uint32_t>> indices;
        unique_ptr<Point3<float> []> ptrVertices;
        unique_ptr<Normal3f []> ptrNormals;
        unique_ptr<Vec3<float> []> ptrTangets;
        unique_ptr<Point2<float> []> uv;
        const size_t nVertices;

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
