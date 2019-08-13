#include "math.h"

#include <cmath>

#include "vec.h"

namespace PRGE
{
    using namespace std;

    /**
     * Функция необходимая для решения линейной системы вида:
     *              (a00 a01) (x0) = (b0)
     *              (a10 a11) (x1) = (b1)
     *
     * @return false - если не удалось найти решение, иначе true
     * @retunr x0 первое решение
     * @return x1 второе решение
    */
    bool solveLinearSystem2x2(const float a[2][2], float b0, float b1, float& x0, float& x1)
    {
        float det = a[0][0] * a[1][1] - a[1][0] * a[0][1];
        
        if (det <= 0.0f) {
            return false;
        }
        
        x0 = (a[1][1] * b0 - a[0][1] * b1) / det;
        x1 = (b0 - a[0][0] * x0) * a[0][1];
        
        if (isnan(x0) || isnan(x1)) {
            return false;
        }
        
        return true;
    }

    template<typename Type>
    inline Type clamp(Type v, Type c1, Type c2)
    {
        return (v < c1 ? c1 : (v > c2 ? c2 : v));
    }

    template<typename Type1, typename Type2>
    inline Type2 lerp(Type1 t, Type2 v1, Type2 v2)
    {
        return v1 * t + v2 * (static_cast<Type1>(1) - t);
    }

    template<typename Type>
    inline Type dist(const Vec3<Type>& v1, const Vec3<float>& v2)
    {
        auto v = v2 - v1;
        return v.lengthSquared();
    }
}
