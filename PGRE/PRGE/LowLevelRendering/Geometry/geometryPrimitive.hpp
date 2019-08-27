//
//  geometryPrimitive.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 27/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef geometryPrimitive_hpp
#define geometryPrimitive_hpp

#include "../../Core/core.h"

#include "primitive.hpp"

#include "SubmissionShape/shape.hpp"

#include <memory>

using namespace std;

namespace PRGE
{
    /**
     * Данный класс представляет фигуру (например сферу или треугольник) 
     * в сцене.
     * 
     * В сцене для каждой фигуры выделяется один экземпляр типа GeometryPrimitive.
    */
    class GeometryPrimitive: 
        public IPrimitive
    {
    public:
        /**
         * Конструктор.
         * 
         * TODO: в качестве аргументов так же должны будут передоваться указатели
         * на Material и ArenaLight.
         * 
         * @param ptrShape указатель на фигуру
        */
        inline GeometryPrimitive(const shared_ptr<IShape>& ptrShape) :
            _ptrShape{ptrShape}
        {}

        /**
         * Данный виртуальный метод отвечает за вычисление
         * ограничивающего объёма в мировом пространстве примитива.
         * 
         * @return ограничивающий объём
        */
        virtual BoundingVolume3<float> worldBound() const NOEXCEPT_PRGE;

        /**
         * Данный виртуальный метод отвечает за вычисление
         * того, пересекает ли луч данный примитив.
         * 
         * @param ray луч
         * @return surfaceInteraction информация о геометрии поверхности
         * @return true - в случае если луч пересёк поверхность, иначе false
        */
        virtual bool intersect(const Ray& ray, SurfaceInteraction& surfaceInteraction) const NOEXCEPT_PRGE;

        /**
         * Данный виртуальный метод отвечает за вычисление
         * того, пересекает ли луч данный примитив.
         * 
         * @param ray луч
         * @return true - в случае если луч пересёк поверхность, иначе false
        */
        virtual bool intersect(const Ray& ray) const NOEXCEPT_PRGE;

        /**
         * TODO: Добавить методы getAreaLight и getMaterial
        */

    private:
        shared_ptr<IShape> _ptrShape;

        /**
         * TODO: Так же необходимо будет добавить указатели на такие классы как Material и ArenaLight.
        */
    };
}

#endif /* geometryPrimitive_hpp */
