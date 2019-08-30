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

using namespace PRGE;

int main()
{
    const AnimatedTransform at (nullptr, nullptr, 1, 2);
    Point3<float> point;
    IntervalArithmetic<float> ia {0.f, 1.f};
    IntervalArithmetic<float> io {0.f, 1.f};
    
    io = ia * io;
    
    at.interpolate(32.54f);
    at.boundingMoutionPoint(point);
    return 0;
}

