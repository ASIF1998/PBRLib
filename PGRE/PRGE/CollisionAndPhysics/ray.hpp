//
//  ray.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 13/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Ray_hpp
#define Ray_hpp

#include "../Core/core.h"

#include "../Core/Math/point.h"
#include "../Core/Math/vec.h"

using namespace std;

namespace PRGE
{
    template<typename>
    class BoundingVolume3;

    class Ray
    {
        friend class Transform;
        friend class BoundingVolume3<float>;
        friend class GeometryPrimitive;
        friend class AnimatedTransform;
        friend class Triangle;
        friend class BVH;

    public:
        Ray() noexcept;
        inline Ray(const Ray& ray) NOEXCEPT_PRGE;
        Ray(Ray&&) NOEXCEPT_PRGE;

        /**
         * Конструктор.
         * 
         * @param o точка начала луча
         * @param dir направления луча
         * @param time время
         * @param tMax максимальное время
        */
        Ray(const Point3<float>& o, Vec3<float>& dir, float time, float tMax) NOEXCEPT_PRGE;

        Ray& operator = (const Ray&) = delete;
        Ray& operator = (Ray&&) = delete;

        inline Point3<float> operator () (float t) const noexcept;

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const Ray& ray);
#endif

    protected:
        Point3<float> _o;
        Vec3<float> _dir;
        float _time;
        mutable float _tMax;
    };

    /**
     * Данный класс содержит дополнительную информацию о 2-х вспомогательных лучах.
     * Эти дополнительные лучи представляют собой лучи камеры, смещенные одним 
     * образцом в направлении от основного луча на плоскости пленки. Определив область, 
     * на которую эти три луча проецируются на затеняемый объект, Texture может 
     * вычислить среднюю площадь для надлежащего сглаживания.
    */
    class RayDifferential :
        public Ray
    {
        friend class Transform;
        friend class AnimatedTransform;

    public:
        RayDifferential();
        RayDifferential(const RayDifferential& rayDifferential) NOEXCEPT_PRGE;
        RayDifferential(RayDifferential&& rayDifferential) NOEXCEPT_PRGE;

        /**
         * Конструктор.
         * 
         * @param o точка начала луча
         * @param dir направления луча
         * @param time время
         * @param tMax максимальное время
        */
        RayDifferential(Point3<float>& o, Vec3<float>& dir, float time = 0.0f, float tMax = numeric_limits<float>::infinity());

        RayDifferential& operator = (const RayDifferential&) = delete;
        RayDifferential& operator = (RayDifferential&&) = delete;

        void scaleDifferentional(float s) NOEXCEPT_PRGE;

#if DEBUG_PRGE == 1
        friend ostream& operator << (ostream& os, const RayDifferential& rayDifferential);
#endif

    private:
        bool _hasDifferentials;
        Point3<float> _rxOrigin;
        Point3<float> _ryOrigin;
        Vec3<float> _rxDirection;
        Vec3<float> _ryDirection;
    };
}

#endif /* ray_hpp */
