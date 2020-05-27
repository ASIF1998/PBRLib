//
//  Scene.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 15/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <memory>
#include <vector>
#include <unordered_map>

#include <string>
#include <string_view>

#include <typeindex>

#include "../Moving/Transform.hpp"
#include "../Rendering/Geometry/AABB.hpp"

#include "../Rendering/Lights/PointLight.hpp"
#include "../Rendering/Lights/SpotLight.hpp"
#include "../Rendering/Lights/DirectionLight.hpp"

#include "../Rendering/Camera/PerspectiveCamera.hpp"

using namespace std;

namespace pbrlib
{
    class Component;
    class Script;
    
    using PtrComponent              = shared_ptr<Component>;
    using PtrScript                 = shared_ptr<Script>;

    class Scene
    {
    public:
        class Node;

        using PtrNode = shared_ptr<Node>;

        class Node
        {
        public:
            Node(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
            );

            virtual ~Node();

            AABB&                   getWorldAABB()      noexcept;
            const AABB&             getWorldAABB()      const noexcept;
            Node*                   getParent()         noexcept;
            const Node*             getParent()         const noexcept;
            PtrNode&                getChild(size_t i);
            const PtrNode&          getChild(size_t i)  const;
            vector<PtrNode>&        getChildren()       noexcept;
            const vector<PtrNode>&  getChildren()       const noexcept;
            Transform&              getLocalTransform() noexcept;
            const Transform&        getLocalTransform() const noexcept;
            Transform&              getWorldTransform() noexcept;
            const Transform&        getWorldTransform() const noexcept;
            string&                 getName()           noexcept;
            const string&           getName()           const noexcept;

            template<typename TComponent>
            inline TComponent& getComponent();

            template<typename TComponent>
            inline const TComponent& getComponent() const;

            template<typename TScript>
            inline TScript& getScript();

            template<typename TScript>
            inline const TScript& getScript() const;

            template<typename TComponent>
            bool hasComponent() const;

            template<typename TScript>
            bool hasScript() const;

            void setParent(Node* ptr_parent) noexcept;
            void setChildren(vector<PtrNode>&& children);
            void setChildren(const vector<PtrNode>& children);
            void setLocalTransform(const Transform& transform);
            void setWorldTransform(const Transform& transform);
            void setWorldAABB(const AABB& bbox);
            void setName(const string_view name);

            bool worldTransformIsCurrent()              const noexcept;
            void worldTransformIsCurrent(bool current)  noexcept;
            bool worldAABBIsCurrent()                   const noexcept;
            void worldAABBIsCurrent(bool current)       noexcept;

            void addComponent(const PtrComponent& ptr_component);
            void addScript(const PtrScript& ptr_script);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            void addChild(PtrNode&& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param child ссылка на дочерний узел.
            */
            void addChild(const PtrNode& child);

            /**
             * @brief Метод необходимый для добавления дочернего узла.
             * @details 
             *      При добавлении дочернего узла указатель на родителя 
             *      устанавливается автоматически.
             * 
             * @param node_name название дочернего узла.
             * @return Указатель на дочерний узел.
            */
            PtrNode& addChild(const string_view node_name);

            void detachChild(const PtrNode& ptr_node);
            void detachChild(const string_view name);

            virtual void update(float delta_time, const Transform& world_transform);

            static PtrNode make(
                const string_view   name    = "No name",
                Node*               parent  = nullptr
            );

        protected:
            Node*                                           _ptr_parent; 
            vector<PtrNode>                                 _ptr_children;
            Transform                                       _local_transform;
            Transform                                       _world_transform;
            bool                                            _world_transform_is_current;
            bool                                            _world_aabb_is_current;
            AABB                                            _world_bbox;
            unordered_map<type_index, PtrComponent>         _components;
            unordered_map<type_index, PtrScript>            _scripts;
            string                                          _name;
        };

    public:
        Scene(const string_view name = "Scene");
        
        void setRootNode(const PtrNode& node);
        void setRootNode(PtrNode&& node);
        void setName(const string_view name);

        PtrNode&         getRootNode()   noexcept;
        const PtrNode&   getRootNode()   const noexcept;
        string&          getName()       noexcept;
        const string&    getName()       const noexcept;

        /**
         * @brief Метод создающий узел для точечного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrNode& makePointLight(
            const PointLight::Builder&   light_builder,
            const string_view            name = "Point Light"
        );

        /**
         * @brief Метод создающий узел для прожекторного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrNode& makeSpotLight(
            const SpotLight::Builder&   light_builder,
            const string_view            name = "Spot Light"
        );

        /**
         * @brief Метод создающий узел для направленного источника света.
         * @details 
         *      Регистрирует источник света в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         * 
         * @param light_builder объект создающий источник света.
         * @param name          название узла.
         * @return Указатель на узел.
        */
        PtrNode& makeDirectionLight(
            const DirectionLight::Builder&  light_builder,
            const string_view               name = "Spot Light"
        );

        /**
         * @brief Метод создающий узел для камеры.
         *  @details 
         *      Регистрирует камеру в сцене.
         *      Созданный узел не имеет родителя и дочерних узлов.
         *      Камера в сцене может быть только одно, поэтому при 
         *      вызове данного метод, предыдущая камера, если она была,
         *      будет заменена на новую.
         * @param camera_builder    объект создающий камеру.
         * @param name              название узла.
         * @return Указатель на узел.
        */
        PtrNode& makeCamera(
            const PerspectiveCamera::Builder&   camera_builder,
            const string_view                   name = "Camera"
        );

        void update(float delta_time);

    private:
        PtrNode         _ptr_root_node;
        PtrNode         _ptr_camera_node;
        vector<PtrNode> _dir_light_nodes;
        vector<PtrNode> _spot_light_nodes;
        vector<PtrNode> _point_light_nodes;
        string          _name;
    };
}

#include "Scene.inl"

#endif /* Scene_hpp */
