#include "Core/core.h"
#include "Core/Math/math.hpp"
#include "Core/Math/vec.h"
#include "Core/Math/point.h"
#include "Core/Math/normal.h"
#include "Core/Math/matrix.h"
#include "Core/Math/intervalAreithmetic.hpp"

#include "CollisionAndPhysics/ray.hpp"

#include "Transform/transform.hpp"
#include "Transform/animatedTransform.hpp"

#include "Core/boundingVolume.hpp"

#include "CollisionAndPhysics/Interaction/interaction.hpp"
#include "CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

#include "LowLevelRendering/Geometry/SubmissionShape/shape.hpp"
#include "LowLevelRendering/Geometry/geometryPrimitive.hpp"

#include "Core/Memory/memoryArena.hpp"
#include "Core/Memory/STLMemory.hpp"

#include "LowLevelRendering/Geometry/SubmissionShape/shape.hpp"
#include "LowLevelRendering/Geometry/SubmissionShape/triangle.hpp"

#include "Core/CullingOptimization/bvh.hpp"

using namespace PRGE;

int main()
{
    shared_ptr<IPrimitive> ptr {new GeometryPrimitive {nullptr}};
    vector<shared_ptr<IPrimitive>> prims;
    
    prims.push_back(ptr);
    
    BVH bvh {prims, 12};
    
    return 0;
}

