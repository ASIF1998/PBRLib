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

#include "LowLevelRendering/SubmissionShape/shape.hpp"

#include "Core/Memory/memoryArena.hpp"
#include "Core/Memory/STLMemory.hpp"

#include <vector>

using namespace PRGE;

int main()
{
    
    STLAllocator<size_t> callocator;
    vector<size_t, STLAllocator<size_t>> ty;
    
    for(size_t i{0}; i < 20; i++) {
        ty.push_back(i);
    }
    
    
    auto alloc = ty.get_allocator();
    
    cout << alloc.capacity() << endl;
    
    for (auto&& iter: ty) {
        cout << iter << endl;
    }
    
    return 0;
}

