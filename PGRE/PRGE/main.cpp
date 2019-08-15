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
    Point2<float> v1;
    Point2<float> v2;
    BoundingVolume2<float> bbox {v1, v2};
    BoundingVolume2<float> bboxx {bbox};
    BoundingVolume3<float> bb3;
    BoundingVolume3<float> bb32;
    
    bb3 == bb32;
    
    bb32.diagonal();
    bb32.surfaceArea();
    bb32.stretchingInTheCoordinateAxis();

    Point2<float> p;
    
    bbox = bboxx;
    bbox.offset(p);
    
    intersect(bb32, bb32);
    
    
    return 0;
}
