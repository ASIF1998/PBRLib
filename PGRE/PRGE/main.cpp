#include "Core/core.h"
#include "Core/Math/vec.h"
#include "Core/Math/point.h"
#include "Core/Math/normal.h"
#include "Core/Math/matrix.h"

#include "Physics/ray.hpp"

#include "Transform/transform.hpp"

using namespace PRGE;

int main()
{

    Point2<float> f1 {0.5f, 0.5f};
    Point2<float> f2 {0.5f, 0.5f};
    Vec2<float> v {0.5f, 0.5f};
    Point3<float> dsd {0.5, 0.5, 0.5};
    Point3<float> ddsd {0.5, 0.5, 0.5};
    
    Ray ray;

    Matrix4x4f m;

    Transform transform;

    cout << m.det() << endl;

    dsd * 0.345;
    return 0;
}
