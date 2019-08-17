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

struct Tri :
    public IShape
{
public:
    Tri(const Transform& objectToWorld, const Transform& worldToObject, bool reverseOrientation, bool transformSwapsHandednes) NOEXCEPT_PRGE :
        IShape(objectToWorld, worldToObject, reverseOrientation, transformSwapsHandednes)
    {}
    
//    Tri(const Tri& tri) :
//        IShape(*static_cast<IShape*>(&tri))
//    {}
    
    virtual ~Tri()
    {}
};

int main()
{
    Vec3<float> v1;
    Vec3<float> v2;
    
    //auto v3 = cross(v1, v2);
//    faceforward(v1, v2);
    
    //Tri ttrtrri {Transform{}, Transform{}, true, true};
    
//    Transform tr1;
//    Transform tr2;
//    
//    Tri t (tr1, tr2, true, true);
    
    //Tri tt{t};

    
    SurfaceInteraction si;
    
    return 0;
}
