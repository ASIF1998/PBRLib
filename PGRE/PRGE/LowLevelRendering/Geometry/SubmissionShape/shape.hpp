//
//  shape.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 14/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef shape_hpp
#define shape_hpp

#include "../../../Core/core.h"

#include "../../../Transform/transform.hpp"

#include "../../../Core/boundingVolume.hpp"

#include "../../../CollisionAndPhysics/ray.hpp"

#include "../../../CollisionAndPhysics/Interaction/interaction.hpp"
#include "../../../CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

#include "../../../Core/Math/point.h"
#include "../../../Core/Math/vec.h"

namespace PRGE
{
    /**
     * Все фигуры определены в координатном простанстве объекта.
     * Например все сферы определные в системе координат, где центр
     * сферы находится в начале системы координат.Для перехода
     * из системы координат объекта в мировую систему координат
     * необходимо воспользовать трансформацией objectToWorld.
     * Для обратного преобразования необходимо воспользовать
     * трансформацией worldToObject.
     *
     * @field objectToWorld трансформация для перехода из системы координат объекта в мировую систему координат
     * @field worldToObject трансформация для перехода из мировой системы координат в системы координат объекта
     * @field reverseOrientation указывает, следует ли обратить вектор нормали в обратном направлении.
    */
    struct IShape
    {
        IShape(const Transform& objectToWorld, const Transform& worldToObject, bool reverseOrientation) NOEXCEPT_PRGE;

        IShape(const IShape&) = delete;
        IShape(IShape&&) = delete;

        IShape& operator = (const IShape&) = delete;
        IShape& operator = (IShape&&) = delete;

        /**
         * Чисто виртуальный метод вычисляющий ограничивающий объём фигуры в пространстве объекта.
         * 
         * @return ограничивающий объём фигуры в пространсте объекта
        */
        virtual BoundingVolume3<float> objectBound() const NOEXCEPT_PRGE = 0;

        /**
         * Чисто виртуальный метод вычисляющий ограничивающий объём фигуры в мировом пространстве.
         * 
         * @return ограничивающий объём фигуры в мировом пространстве
        */
        virtual BoundingVolume3<float> worldBound() const NOEXCEPT_PRGE = 0;

        /**
         * Чисто виртуальный метод вычисляющий пересеччение луча и фигуры.
         * 
         * @param ray луч
         * @return tHit параметр с помощью которого можно определить ближайшую точку пересечения луча и фигуры
         * @retrun surfaceInteraction  информация о геометри места пересечения
         * @return true - если есть пересечение, иначе false
        */
        virtual bool intersect(const Ray& ray, float* tHit, SurfaceInteraction* surfaceInteraction, bool testAlphaTexture = true) const NOEXCEPT_PRGE = 0;

        /**
         * Чисто виртуальный метод вычисляющий пересеччение луча и фигуры.
         * 
         * @param ray луч
         * @return true - если есть пересечение, иначе false
        */
        virtual bool intersect(const Ray& ray, bool testAlphaTexture = true) const NOEXCEPT_PRGE = 0;

        /**
         * Чисто виртуальный метод вычисляющий площадь поверхности фигуры.
         * 
         * @return площадь поверхности фигуры
        */
        virtual float arena() const NOEXCEPT_PRGE = 0;

        virtual Interaction sample(const Point2<float>& uv) const = 0;
        virtual Interaction sample(const Interaction& interaction, const Point2<float>& uv) const;

        virtual float pdf(const Interaction& interaction) const;

        virtual ~IShape() {}

    public:
        Transform objectToWorld;
        Transform worldToObject;
        bool reverseOrientation;
        bool transformSwapsHandednes;
    };
}

#endif /* shape_hpp */
