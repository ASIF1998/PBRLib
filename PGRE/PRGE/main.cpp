#include "Core/core.h"
#include "Core/Math/vec.h"
#include "Core/Math/point.h"
#include "Core/Math/normal.h"
#include "Core/Math/matrix.h"

#include "Physics/ray.hpp"

#include "Transform/transform.hpp"

#include "Core/boundingVolume.hpp"

using namespace PRGE;

int main()
{
    Vec2<float> v1;
    Vec2<float> v2;
    BoundingVolume2<float> bbox {v1, v2};
    BoundingVolume2<float> bboxx {bbox};
    Point2<float> p;
    
    bbox = bboxx;
    bbox.offset(p);
    
    
    return 0;
}
