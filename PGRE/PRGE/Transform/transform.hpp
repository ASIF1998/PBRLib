//
//  transform.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 14/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef transform_hpp
#define transform_hpp

#include "../Core/Math/matrix.h"
#include "../Core/Math/vec.h"
#include "../Core/Math/point.h"
#include "../Core/Math/normal.h"

#include "../Core/boundingVolume.hpp"

#include "../CollisionAndPhysics/ray.hpp"
#include "../CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

#include "../Core/core.h"

namespace PRGE
{
    class Transform
    {
    public:
        Transform() NOEXCEPT_PRGE;
        Transform(const Matrix4x4f& transformMatrix) NOEXCEPT_PRGE;
        Transform(const Matrix4x4f& transformMatrix, const Matrix4x4f& inverseTransformMatrix) NOEXCEPT_PRGE;
        Transform(const Transform& transform) NOEXCEPT_PRGE;
        Transform(Transform&& transform) NOEXCEPT_PRGE;

        Transform& operator = (const Transform& transform) NOEXCEPT_PRGE;
        Transform& operator = (Transform&& transform) NOEXCEPT_PRGE;
        inline bool operator == (const Transform& transform) const noexcept;
        inline bool operator != (const Transform& transform) const noexcept;
        inline bool operator < (const Transform& transform) const noexcept;

        Vec3<float> operator () (const Vec3<float>& vec3) const NOEXCEPT_PRGE;
        Point3<float> operator () (const Point3<float>& point3) const NOEXCEPT_PRGE;
        Normal3f operator () (const Normal3f& normal) const NOEXCEPT_PRGE;
        Ray operator () (const Ray& ray) const NOEXCEPT_PRGE;
        RayDifferential operator () (const RayDifferential& rayDifferential) NOEXCEPT_PRGE;
        BoundingVolume3<float> operator () (const BoundingVolume3<float>& boundingVolume3) const NOEXCEPT_PRGE;
        SurfaceInteraction operator () (const SurfaceInteraction& surfaceInteraction) const NOEXCEPT_PRGE;

    private:
        Matrix4x4f _m;
        Matrix4x4f _invM;
    };

    /**
     *  Функция возвращающая трансформацию пермещения.
     *
     * @param t вектор перемещения
     * @return трансформация пермещения
    */
    inline Transform translate(const Vec3<float>& t) NOEXCEPT_PRGE;

    /**
     * Функция вычисляющая трансформацию маштабирования.
     * 
     * @param scaleX маштабирование по оси X
     * @param scaleY маштабирование по оси Y
     * @param scaleZ маштабирование по оси Z
     * @return трансформацию маштабирования
    */
    Transform scale(float scaleX, float scaleY, float scaleZ) NOEXCEPT_PRGE;

    /**
     * Функция вычисляющая трансформацию поворота по оси X
     * 
     * @param angle угол поворота по оси X
     * @return трансформацию поворота по оси X
    */
    Transform rotateX(float angle) NOEXCEPT_PRGE;

    /**
     * Функция вычисляющая трансформацию поворота по оси Y
     * 
     * @param angle угол поворота по оси Y
     * @return трансформацию поворота по оси Y
    */
    Transform rotateY(float angle) NOEXCEPT_PRGE;

    /**
     * Функция вычисляющая трансформацию поворота по оси Z
     * 
     * @param angle угол поворота по оси Z
     * @return трансформацию поворота по оси Z
    */
    Transform rotateZ(float angle) NOEXCEPT_PRGE;

    /**
     * Функция вычисляющая трансформацию поворота по оси axis
     * 
     * @param angle угол поворота по оси axis
     * @param axis произвольная ось
     * @return трансформацию поворота по оси axis
    */
    Transform rotate(float angle, Vec3<float>& axis) NOEXCEPT_PRGE;

    /**
     * Функция возвращающая трансформацию перевода из пространства модели в видовое пространство.
     *
     * @param pos позиция
     * @param look точка, на которую направлен взгляд
     * @param up вектор указывающий на вверх
     * @return трансформация перевода из пространства модели в видовое пространство.
    */
    Transform lookAt(Point3<float>& pos, Point3<float>& look, Vec3<float>& up) NOEXCEPT_PRGE;
}

#endif /* transform_hpp */
