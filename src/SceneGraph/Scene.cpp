//
//  Scene.cpp
//  PBRLib
//
//  Created by Асиф Мамедов on 17/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include "Scene.hpp"

namespace pbrlib
{
    Scene::Node::Node(
        const string_view   name,
        Node*               ptr_parent 
    ) :
        _ptr_parent                 (ptr_parent),
        _world_transform_is_current (false),
        _world_aabb_is_current      (false),
        _name                       (name)
    {}

    Scene::Node::~Node()
    {}

    AABB& Scene::Node::getWorldAABB() noexcept
    {
        return _world_bbox;
    }

    const AABB& Scene::Node::getWorldAABB() const noexcept
    {
        return _world_bbox;
    }

    Scene::Node* Scene::Node::getParent() noexcept
    {
        return _ptr_parent;
    }

    const Scene::Node* Scene::Node::getParent() const noexcept
    {
        return _ptr_parent;
    }

    Scene::PtrNode& Scene::Node::getChild(size_t i)
    {
        return _ptr_children[i];
    }

    const Scene::PtrNode& Scene::Node::getChild(size_t i) const
    {
        return _ptr_children[i];
    }

    vector<Scene::PtrNode>& Scene::Node::getChildren() noexcept
    {
        return _ptr_children;
    }

    const vector<Scene::PtrNode>& Scene::Node::getChildren() const noexcept
    {
        return _ptr_children;
    }

    Transform& Scene::Node::getLocalTransform() noexcept
    {
        return _local_transform;
    }

    const Transform& Scene::Node::getLocalTransform() const noexcept
    {
        return _local_transform;
    }

    Transform& Scene::Node::getWorldTransform() noexcept
    {
        return _world_transform;
    }

    const Transform& Scene::Node::getWorldTransform() const noexcept
    {
        return _world_transform;
    }

    string& Scene::Node::getName() noexcept
    {
        return _name;
    }

    const string& Scene::Node::getName() const noexcept
    {
        return _name;
    }

    void Scene::Node::setParent(Node* ptr_node) noexcept
    {
        _ptr_parent = ptr_node;
    }

    void Scene::Node::setChildren(vector<PtrNode>&& children)
    {
        swap(_ptr_children, children);
    }

    void Scene::Node::setChildren(const vector<PtrNode>& children)
    {
        _ptr_children = children;
    }

    void Scene::Node::setLocalTransform(const Transform& transform)
    {
        _local_transform = transform;
    }

    void Scene::Node::setWorldTransform(const Transform& transform)
    {
        _world_transform_is_current = true;
        _world_transform            = transform;
    }

    void Scene::Node::setWorldAABB(const AABB& bbox)
    {
        _world_aabb_is_current = true;
        _world_bbox = bbox;
    }

    void Scene::Node::setName(const string_view name)
    {
        _name = name;
    }

    bool Scene::Node::worldTransformIsCurrent() const noexcept
    {
        return _world_transform_is_current;
    }

    void Scene::Node::worldTransformIsCurrent(bool current) noexcept
    {
        _world_transform_is_current = current;
    }

    bool Scene::Node::worldAABBIsCurrent() const noexcept
    {
        return _world_aabb_is_current;
    }

    void Scene::Node::worldAABBIsCurrent(bool current) noexcept
    {
        _world_aabb_is_current = current;
    }

    void Scene::Node::addNodeModifier(INodeModifier* ptr_node_modifier)
    {
        auto it = _node_modifiers.find(ptr_node_modifier->getType());

        if (it != end(_node_modifiers)) {
            it->second.reset(ptr_node_modifier);
        } else {
            _node_modifiers.insert(make_pair(ptr_node_modifier->getType(), ptr_node_modifier));
        }
    }

    void Scene::Node::addChild(PtrNode&& child)
    {
        child->setParent(this);
        _ptr_children.push_back(move(child));
    }

    void Scene::Node::addChild(const PtrNode& child)
    {
        child->setParent(this);
        _ptr_children.push_back(child);
    }

    Scene::PtrNode& Scene::Node::addChild(const string_view node_name)
    {
        PtrNode child = make_shared<Node>(node_name);

        child->setParent(this);
        _ptr_children.push_back(child);
        return _ptr_children.back();
    }

    void Scene::Node::update(float delta_time, const Transform& transform)
    {
        for (auto s{begin(_node_modifiers)}, e{end(_node_modifiers)}; s != e; s++) {
            s->second->update(this, delta_time);
        }

        if (!_world_transform_is_current) {
            _world_transform = transform;
        }

        if (!_ptr_children.empty())  {
            Transform children_world_transform = _world_transform * _local_transform;

            for (size_t i{0}, size{_ptr_children.size()}; i < size; i++) {
                _ptr_children[i]->update(delta_time, children_world_transform);
            }

            if (!_world_aabb_is_current) {
                _world_bbox = _ptr_children[0]->getWorldAABB();

                for (size_t i{1}, size{_ptr_children.size()}; i < size; i++) {
                    _world_bbox = AABB::aabbUnion(_world_bbox, _ptr_children[i]->getWorldAABB());
                }
            }
        }
    }

    Scene::PtrNode Scene::Node::make(const string_view name, Node* parent)
    {
        return make_shared<Node>(name, parent);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    INodeModifier::INodeModifier(const string_view name) :
        _name(name)
    {}

    void INodeModifier::setName(const string_view name)
    {
        _name = name;
    }

    string& INodeModifier::getName() noexcept
    {
        return _name;
    }

    const string& INodeModifier::getName() const noexcept
    {
        return _name;
    }

    INodeModifier::~INodeModifier()
    {}
}