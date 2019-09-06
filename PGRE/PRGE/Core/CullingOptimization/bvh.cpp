//
//  bvh.cpp
//  PRGE
//
//  Created by Асиф Мамедов on 05/09/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "bvh.hpp"

#include "../boundingVolume.hpp"

#include "../Math/point.h"

namespace PRGE
{
    template<class Predicate, class ForwardIterator>
    ForwardIterator partition(ForwardIterator first, ForwardIterator last, Predicate pred)
    {
        ForwardIterator temp;
        while (true) {
            if (first == last) {
                return first;
            }

            if (!pred(*first)) {
                break;
            }

            first++;

            for (auto p{first}; ++p != last;) {
                if (pred(*p)) {
                    temp = first;
                    first = p;
                    p = temp;
                    first++;
                }
            }
        }

        return first;
    }

    struct LinearBVH
    {
        BoundingVolume3<float> bounds;
        union
        {
            size_t primitivesOffset;
            size_t secondChildOffset;
        };

        uint16_t numPrimitives;
        uint8_t axis;
        uint8_t pad[1];
    };

    /**
     * @feild count количество примитивов в ведре
     * @feild bounds коробка описывающая объём ведра
    */
    struct BucketInfo
    {
        inline BucketInfo() :
            count{0}
        {}
        
        int count;
        BoundingVolume3<float> bounds;
    };

    /**
     * Данная структура хранит информацию о примитиве.
     * 
     * @feild primitiveNumber индекс примитива в массиве _primitives в классе BVH
     * @feild bounds ограничивающая коробка в мировом пространстве
     * @feild centroid центр коробки bounds
    */
    struct BVHPrimitiveInfo
    {
        BVHPrimitiveInfo()
        {}

        BVHPrimitiveInfo(size_t primitiveNumber, BoundingVolume3<float> bounds) :
            primitiveNumber{primitiveNumber},
            bounds{bounds},
            centroid{bounds._pMin * 0.5 + bounds._pMax * 0.5}
        {}

        BVHPrimitiveInfo(const BVHPrimitiveInfo& p) :
            primitiveNumber{p.primitiveNumber},
            bounds{p.bounds},
            centroid{p.centroid}
        {}

        BVHPrimitiveInfo& operator = (const BVHPrimitiveInfo& primitiveInfo)
        {
            return *this;
        }

        BVHPrimitiveInfo(BVHPrimitiveInfo&& p) :
            primitiveNumber{p.primitiveNumber},
            bounds{p.bounds},
            centroid{p.centroid}
        {}

        size_t primitiveNumber;
        BoundingVolume3<float> bounds;
        Point3<float> centroid;
    };

    /**
     * Данная структура описывает узел дерева.
     * 
     * @feild bounds ограничивающая коробка представляющая границы всех дочерних узлов
     * @feild childrens дочерние узлы данного узла
     * @feild splitAxis индекс координатной оси вдоль которой был разделён данный узел
     * @feild firslPrimOffset индекс в массиве _primitives класса BVH
     * @feild numPrimitives количество примитивов принадлежащий данному узлу
    */
    struct BVHBuildNode
    {
        /**
         * Метод предназначенный для инициализации листьев в дереве.
         * 
         * @param first индекс первого примитива в массиве _primitives класса BVH пренадлежащего данному листу
         * @param n количество примитивов у данного листа
         * @param b ограничивающая коробка представляющая границы всех дочерних узлов
        */
        void initLeaf(size_t first, size_t n, const BoundingVolume3<float>& b)
        {
            firstPrimitivesOffset = first;
            numPrimitives = n;
            bounds = b;
            childrens[0] = childrens[1] = nullptr;
        }

        /**
         * Метод необходимый для инициализации внутренего узла.
         * 
         * @param axis индекс координатной оси вдоль которой осуществлялось разделение
         * @param c0 левуй дочерний узел (предпологается что он уже создан)
         * @param c1 правый дочерний узел (предпологается что он уже создан)
        */
        void initInterior(unsigned long axis, BVHBuildNode* c0, BVHBuildNode* c1)
        {
            childrens[0] = c0;
            childrens[1] = c1;
            bounds = merge(c0->bounds, c1->bounds);
            splitAxis = axis;
            numPrimitives = 0;
        }

        BoundingVolume3<float> bounds;
        BVHBuildNode* childrens[2];
        unsigned long splitAxis;
        size_t firstPrimitivesOffset;
        size_t numPrimitives;
    };

    BVH::BVH(const vector<shared_ptr<IPrimitive>>& primitives, uint32_t maxPrimitivesInNode) :
        _maxPrimitiveInNode{maxPrimitivesInNode},
        _primitives{primitives}
    {
        if (!_primitives.size()) {
            return ;
        }

        vector<BVHPrimitiveInfo> primitiveInfo;

        primitiveInfo.reserve(_primitives.size());

        for (size_t i{0}; i < _primitives.size(); i++) {
            primitiveInfo.push_back({i, _primitives[i]->worldBound()});
        }
        
        /// Общее количество созданных узлов.
        int totalNodes = 0;

        vector<shared_ptr<IPrimitive>> orderPrims;
        BVHBuildNode* root = recursiveBuild(primitiveInfo, 0, primitives.size(), &totalNodes, orderPrims);

        swap(_primitives, orderPrims);
        primitiveInfo.resize(0);

        _nodes = new LinearBVH [totalNodes];
        size_t offset = 0;

        flattenBVH(root, &offset);
    }

    BVH::~BVH()
    {
        delete [] _nodes;
    }

    BVHBuildNode* BVH::recursiveBuild(const vector<BVHPrimitiveInfo>& primitivesInfo, 
                                      size_t start, 
                                      size_t end, 
                                      int* totalNodes, 
                                      vector<shared_ptr<IPrimitive>>& orderPrimitives)
    {
        auto* node = new BVHBuildNode;
        (*totalNodes)++;

        auto numPrimitives = end - start;

        BoundingVolume3<float> bounds;

        for (size_t i{start}; i < end; i++) {
            bounds = merge(bounds, primitivesInfo[i].bounds);
        }

        if (numPrimitives == 1) {
            auto offset = orderPrimitives.size();

            for (size_t i{start}; i < end; i++) {
                orderPrimitives.push_back(_primitives[primitivesInfo[i].primitiveNumber]);
            }

            node->initLeaf(offset, numPrimitives, bounds);
        } else {
            BoundingVolume3<float> centroidBounds;

            for (size_t i{start}; i < end; i++) {
                centroidBounds = merge(centroidBounds, primitivesInfo[i].centroid);
            }

            auto dim = centroidBounds.maxExtent();
            auto mid = (start + end) / 2;

            /// Если все точки центроида находятся в одном и том же положении, то
            /// рекурсия останавливается и создаётся узел с примитивами.
            if (centroidBounds._pMax[dim] == centroidBounds._pMin[dim]) {
                auto offset = orderPrimitives.size();

                for (size_t i{start}; i < end; i++) {
                    orderPrimitives.push_back(_primitives[primitivesInfo[i].primitiveNumber]);
                }

                node->initLeaf(offset, numPrimitives, bounds);

                return node;
            } else {
                constexpr size_t numBuckets = 12;
                BucketInfo buckets[numBuckets];

                for (size_t i{start}; i < end; i++) {
                    int b = numBuckets * centroidBounds.offset(primitivesInfo[i].centroid)[dim];

                    if (b == numBuckets) {
                        b--;
                    }

                    buckets[b].count++;
                    buckets[b].bounds = merge(buckets[b].bounds, primitivesInfo[i].bounds);
                }

                float cost[numBuckets - 1];

                for(size_t i{0}; i < numBuckets - 1; i++) {
                    BoundingVolume3<float> b0, b1;
                    int count0 =0, count1 = 0;

                    for (size_t j{0}; j <= i; j++) {
                        b0 = merge(b0, buckets[j].bounds);
                        count0 += buckets[j].count;
                    }

                    for (size_t j{i}; j < numBuckets; j++) {
                        b1 = merge(b0, buckets[j].bounds);
                        count1 += buckets[j].count;
                    }

                    cost[i] = .125f + (count0 * b0.surfaceArea() + count1 * b1.surfaceArea()) / bounds.surfaceArea();
                }

                float minCost = cost[0];
                size_t minCostSplitBucket = 0;

                for (size_t i{0}; i < numBuckets - 1; i++) {
                    if (cost[i] < minCost) {
                        minCost = cost[i];
                        minCostSplitBucket = i;
                    }
                }

                float leafCost = static_cast<float>(numPrimitives);

                if (numPrimitives > _maxPrimitiveInNode || minCost < leafCost) {
                    auto ptrMid = PRGE::partition(
                        primitivesInfo.begin(),
                        primitivesInfo.end(),
                        [=] (const BVHPrimitiveInfo& primitiveInfo) {
                            int b = numBuckets * centroidBounds.offset(primitiveInfo.centroid)[dim];

                            if (b == numBuckets) {
                                b--;
                            }

                            return b <= minCostSplitBucket;
                        }
                    );

                    mid = &(*ptrMid) - &primitivesInfo[0];
                } else {
                    auto offset = orderPrimitives.size();

                    for (size_t i{start}; i < end; i++) {
                        orderPrimitives.push_back(_primitives[primitivesInfo[i].primitiveNumber]);
                    }

                    node->initLeaf(offset, numPrimitives, bounds);

                    return node;
                }
                

                node->initInterior(dim,
                                   recursiveBuild(primitivesInfo, start, mid, totalNodes, orderPrimitives),
                                   recursiveBuild(primitivesInfo, mid, end, totalNodes, orderPrimitives));
            }
        }

        return node;
    }

    int BVH::flattenBVH(BVHBuildNode* node, size_t* offset)
    {
        auto* linearNode = _nodes + *offset;
        linearNode->bounds = node->bounds;
        size_t myOffset = (*offset)++;

        if (node->numPrimitives > 0) {
            linearNode->primitivesOffset = node->firstPrimitivesOffset;
            linearNode->numPrimitives = node->numPrimitives;
        } else {
            linearNode->axis = node->splitAxis;
            linearNode->numPrimitives = 0;
            flattenBVH(node->childrens[0], offset);
            linearNode->secondChildOffset = flattenBVH(node->childrens[1], offset);
        }

        return 0;
    }
}
