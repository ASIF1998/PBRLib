//
//  math.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 17/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef math_hpp
#define math_hpp

#include "../core.h"

#include <cmath>

#define NAN_OR_INF_XY(x, y) if(isnan(x) || isnan(y) || isinf(x) || isinf(y)) throw invalid_argument("The x and y arguments passed are not valid")
#define NAN_OR_INF_XYZ(x, y, z) if(isnan(x) || isnan(y) || isnan(z) || isinf(x) || isinf(y) || isinf(z)) throw invalid_argument("The x, y and z arguments passed are not valid")
#define NAN_OR_INF_XYZW(x, y, z, w) if(isnan(x) || isnan(y) || isnan(z) || isnan(w) || isinf(x) || isinf(y) || isinf(z) || isinf(w)) throw invalid_argument("The x, y, z and w arguments passed are not valid")

namespace PRGE
{
    template<typename>
    class Vec3;

    /**
     * Функция необходимая для решения линейной системы вида:
     *              (a00 a01) (x0) = (b0)
     *              (a10 a11) (x1) = (b1)
     *
     * @return false - если не удалось найти решение, иначе true
     * @retunr x0 первое решение
     * @return x1 второе решение
     */
    bool solveLinearSystem2x2(const float a[2][2], float b0, float b1, float& x0, float& x1);

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

    /**
     * Функция вычисляет u1 направленный в туже сторону что и u2.
     *
     * @param u1 вектор или нормаль
     * @param u2 вектор или нормаль
     * @return u1 направленный в туже сторону что и u2
    */
    template<typename Type1, typename Type2>
    auto faceforward(const Type1& u1, const Type2& u2)
    {
        return (dot(u1, u2) < 0 ? u1 * -1 : u1);
    }
    
    template<typename Type>
    inline size_t maxDimension(const Type& x) NOEXCEPT_PRGE
    {
        return (x[0] > x[1] ? 0 : (x[1] > x[2] ? 1 : 2));
    }

    template<typename Type1, template<typename> class Type2>
    inline Type2<Type1> abs(const Type2<Type1>& x) NOEXCEPT_PRGE
    {
        return {std::abs(x[0]), std::abs(x[0]), std::abs(x[0])};
    }
}

#endif /* math_hpp */
