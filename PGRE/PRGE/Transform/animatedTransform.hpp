//
//  animatedTransform.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 30/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef animatedTransform_hpp
#define animatedTransform_hpp

#include "../Core/core.h"

#include "transform.hpp"

#include "../Core/Math/vec.h"
#include "../Core/Math/quaternion.h"
#include "../Core/Math/math.hpp"
#include "../Core/Math/point.h"
#include "../Core/Math/intervalAreithmetic.hpp"

#include "../CollisionAndPhysics/ray.hpp"

#include "../Core/boundingVolume.hpp"

namespace PRGE
{
    /**
     * Данная структура описывает один из коэффициентов производной
     * от функции движения.
    */
    struct DerivativeCoefficient
    {
        inline float calc(const Point3<float>& point) const NOEXCEPT_PRGE
        {
            return kx * point[0] * ky * point[1] * kz * point[2] + kc;
        }

        float kx;
        float ky;
        float kz;
        float kc;
    };

    /**
     * Данный класс отвечает за выдачу промежуточгой трансформации
     * между ptrStartTransform и ptrEndTransform с помощью интерполяции.
    */
    class AnimatedTransform
    {
    public:
        /**
         * Конструктор.
         * 
         * @param ptrStartTransform указатель на начальную трансформацию
         * @param ptrEndTransform указатель на конечную трансформацию
         * @param startTime время начала трансформации 
         * @param endTime время окончания трансформации
        */
        AnimatedTransform(const Transform* ptrStartTransform, const Transform* ptrEndTransform, float startTime, float endTime);

        /**
         * Статический метод осуществляющий разложение матрицы на 3-и компоненты:
         *  - перевод;
         *  - поворот;
         *  - маштабирование.
         * 
         * Разложение осуществляется с помощью техники полярного разложения.
         * 
         * @return translate перевод
         * @return rotate поворот
         * @return scale маштабирование
        */
        void static decompose(const Matrix4x4f& m, Vec3<float>& translate, Quaternion& rotate, Matrix4x4f& scale);

        /**
         * Метод интерполяции для трансформации, который выдаёт проежуточный результат
         * между начальной и конечной трансформациями.
         * 
         * @param time время
         * @return проежуточный результат между начальной и конечной трансформациями
        */
        Transform interpolate(float time) const;

        /**
         * Метод вычисляющий ограничивающий объём экземпляров типа BoundingVolume3<float>,
         * т. к. они то же меняют своё местополоэение во время анимации.
         * 
         * @param boundingVolume огрничивающий объём
         * @return ограничивающий объём boundingVolume охватывающий траекторию движения
        */
        BoundingVolume3<float> moutionBounds(const BoundingVolume3<float>& boundingVolume) const;

        /**
         * Метод вычисляющий ограничивающий объём, который включает в себя всю траекторию
         * движения точки point.
         * 
         * @param point точка
         * @return ограничивающий объём включающий траекторию движения точки point
        */
        BoundingVolume3<float> boundingMoutionPoint(const Point3<float>& point) const;

        Vec3<float> operator () (const Vec3<float>& vec, float time) const;
        Point3<float> operator () (const Point3<float>& point, float time) const;
        Normal3f operator () (const Normal3f& normal, float time) const;
        Ray operator () (const Ray& ray) const;
        RayDifferential operator () (const RayDifferential& rayDifferential) const;

    private:
        /**
        * @field _ptrStartTransform указатель на начальную трансформацию
        * @field _ptrEndTransform указатель на конечную трансформацию
        * @field _startTime время начала трансформации
        * @field _endTime время конца трансформации
        * @field _actualAnimated равняется true в случае если ptrStartTransform и ptrEndTransform не равны
        * @field _hasRotate true - в случае если осуществляется поворот, иначе false
        * @field _translates массив из двух перемещений между которыми будет происходить линейная интерполяция
        * @field _rotates массив из двух поворотов между которыми будет происходить сферическая линейная интерполяция
        * @field _scales массив из двух маштабирований между которыми будет происходить линейная интерполяция
        */

        const Transform* _ptrStartTransform;
        const Transform* _ptrEndTransform;
        float _startTime;
        float _endTime;
        bool _actualAnimated;
        bool _hasRotate;
        Vec3<float> _translates [2];
        Quaternion _rotates [2];
        Matrix4x4f _scales [2];
        DerivativeCoefficient _c1[3];
        DerivativeCoefficient _c2[3];
        DerivativeCoefficient _c3[3];
        DerivativeCoefficient _c4[3];
        DerivativeCoefficient _c5[3];
    };
}

#endif /* animatedTransform_hpp */
