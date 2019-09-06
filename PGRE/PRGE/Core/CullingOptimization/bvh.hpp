//
//  bvh.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 05/09/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef bvh_hpp
#define bvh_hpp

#include "../../LowLevelRendering/Geometry/primitive.hpp"

#include "../../CollisionAndPhysics/ray.hpp"

#include "../../CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

#include <memory>

#include <vector>

#include <cstdint>

using namespace std;

namespace PRGE
{
    struct BVHPrimitiveInfo;
    struct BVHBuildNode;
    struct LinearBVH;

    class BVH
    {
    public:
        BVH(const vector<shared_ptr<IPrimitive>>& primitives, uint32_t maxPrimitivesInNode);
        ~BVH();

        virtual void intersect(const Ray& ray, SurfaceInteraction* surfaceInteraction) const;

    private:
        BVHBuildNode* recursiveBuild(const vector<BVHPrimitiveInfo>& primitivesInfo, 
                                     size_t start, 
                                     size_t end, 
                                     int* totalNodes, 
                                     vector<shared_ptr<IPrimitive>>& orderPrimitives);

        size_t flattenBVH(BVHBuildNode* node, size_t* offset);

    private:
        uint32_t _maxPrimitiveInNode;
        vector<shared_ptr<IPrimitive>> _primitives;
        LinearBVH* _nodes;
    };
}

#endif /* bvh_hpp */
