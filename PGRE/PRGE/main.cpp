#include "Core/core.h"
#include "Core/Math/math.hpp"
#include "Core/Math/vec.h"
#include "Core/Math/point.h"
#include "Core/Math/normal.h"
#include "Core/Math/matrix.h"

#include "CollisionAndPhysics/ray.hpp"

#include "Transform/transform.hpp"

#include "Core/boundingVolume.hpp"

#include "CollisionAndPhysics/Interaction/interaction.hpp"
#include "CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

#include "LowLevelRendering/Geometry/SubmissionShape/shape.hpp"
#include "LowLevelRendering/Geometry/geometryPrimitive.hpp"

#include "Core/Memory/memoryArena.hpp"
#include "Core/Memory/STLMemory.hpp"

#include <vector>

using namespace PRGE;

int main()
{
//    try {
//
//
//        //STLAllocator<int> alloc (3000);
//
//        vector<int, STLAllocator<int>> vec(400);
//        const auto&& all = vec.get_allocator();
//
//        for (int i{0}; i < 500; i++) {
//            cout << i << endl;
//            vec.push_back(i);
//        }
//
//    } catch (const exception& ex) {
//        cerr << ex.what() << endl;
//    }
    
    shared_ptr<IShape> i = nullptr;
    
    SurfaceInteraction s;

    //GeometryPrimitive gp ;

    GeometryPrimitive gp {nullptr};
    
    IPrimitive* ptrGP = &gp;
    
    gp.intersect(Ray{}, s);
    ptrGP->intersect(Ray{});
    
    return 0;
}

