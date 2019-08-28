//
//  triangle.hpp
//  PRGE
//
//  Created by Асиф Мамедов on 28/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef triangle_hpp
#define triangle_hpp

#include "shape.hpp"

#include "../../mesh.hpp"

#include "../../../Transform/transform.hpp"

#include <memory>

namespace PRGE
{
    class Triangle :
        public IShape
    {
    public:
        /**
         * Конструктор.
         * 
         * @param objectToWorld преобразование из пространства объекта в мировое пространство
         * @param worldToObject преобразование из мирового пространства в пространства объекта
         * @param reservOrientation указывает, следует ли обратить вектор нормали в обратном направлении
        */
        inline Triangle(const Transform& objectToWorld,
                        const Transform& worldToObject,
                        bool reservOrientation, 
                        const shared_ptr<Mesh>& ptrMesh, 
                        size_t trangleNumber) :
            IShape(objectToWorld, worldToObject, reservOrientation),
            _ptrMesh{ptrMesh},
            _indexInIndecesArray{trangleNumber * 3}
        {}

        virtual BoundingVolume3<float> objectBound() const NOEXCEPT_PRGE override;
        virtual BoundingVolume3<float> worldBound() const NOEXCEPT_PRGE override;
        virtual bool intersect(const Ray& ray, float* tHit, SurfaceInteraction* surfaceInteraction, bool testAlphaTexture = true) const NOEXCEPT_PRGE override;
        virtual bool intersect(const Ray& ray, bool testAlphaTexture = true) const NOEXCEPT_PRGE override;
        virtual float arena() const NOEXCEPT_PRGE override;
        virtual Interaction sample(const Point2<float>& uv) const override;

    private:
        shared_ptr<Mesh> _ptrMesh;
        size_t _indexInIndecesArray;
    };
}

#endif /* triangle_hpp */
