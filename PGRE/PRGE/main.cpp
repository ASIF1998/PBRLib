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

using namespace PRGE;

int main()
{
    Vec3<float> v1;
    Vec3<float> v2;
    Point3<float> p3;
    SurfaceInteraction si;
    BoundingVolume3<float> b1;
    BoundingVolume3<float> b2;
    
    overlabs(b1, b2);
    inside(b1, p3);
    auto bcdf = expand(b1, 12.5f);
    
    return 0;
}
