//
//  primitive.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 27/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef primitive_hpp
#define primitive_hpp

#include "../Core/boundingVolume.hpp"

#include "../CollisionAndPhysics/ray.hpp"
#include "../CollisionAndPhysics/Interaction/surfaceInteraction.hpp"

namespace PRGE
{
    /**
     * Данный абстрактная структура является связующим звеном 
     * между обработкой геометрии и подсистемами затенения
    */
    struct IPrimitive
    {
        virtual ~IPrimitive() noexcept;

        /**
         * Данный чисто виртуальный метод отвечает за вычисление
         * ограничивающего объёма в мировом пространстве примитива.
         * 
         * @return ограничивающий объём
        */
        virtual BoundingVolume3<float> worldBound() const = 0;

        /**
         * Данный чисто виртуальный метод отвечает за вычисление
         * того, пересекает ли луч данный примитив.
         * 
         * @param ray луч
         * @return surfaceInteraction информация о геометрии поверхности
         * @return true - в случае если луч пересёк поверхность, иначе false
        */
        virtual bool intersect(const Ray& ray, SurfaceInteraction& surfaceInteraction) const = 0;

        /**
         * Данный чисто виртуальный метод отвечает за вычисление
         * того, пересекает ли луч данный примитив.
         * 
         * @param ray луч
         * @return true - в случае если луч пересёк поверхность, иначе false
        */
        virtual bool intersect(const Ray& ray) const = 0;

        /**
         * TODO: Данная структура так же должна будет содержать чисто виртуальные
         * методы, возвращабщие класс описывающий распредение излучение примитива
         * (AreaLight) и класс описывающий материал (Material).
         * 
         * Помимо этого должен присутствовать метод computeSactteringFunction()
         * который инициализирует представления светорассеивающих свойств материала
         * в точке пересечения на поверхности(например инициализация BRDF, BSSRDF
         * (подповерхностное рассеивание)). Так же данный метод будет принимать 
         * MemoryArena и перечеслитель TransportMode, который указывает, начался ли 
         * путь луча от камеры или от источника света.
        */
    };
}

#endif /* primitive_hpp */
