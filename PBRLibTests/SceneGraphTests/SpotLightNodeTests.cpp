//
//  SpotLightNodeTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 19/05/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <ctime>

#include <gtest/gtest.h>

#include "../../src/SceneGraph/Scene.hpp"
#include "../../src/SceneGraph/SpotLightNode.hpp"

using namespace testing;
using namespace pbrlib;
using namespace std;

struct TestSpotLightNodeModifier :
    public ISpotLightNodeModifier
{
    TestSpotLightNodeModifier() :
        ISpotLightNodeModifier  (),
        i                       (0)
    {}

    virtual void update(SpotLightNode* ptr_node, float delta_time) override
    {}

    virtual type_index  getType() const override
    {
        return INodeModifier::getTypeIndex<TestSpotLightNodeModifier>();
    }

    int i;
};

TEST(SceneGraphSpotLightNode, Constructor)
{
    constexpr string_view   name1   = "Spot Light Node";
    const string            name2   = "Node 2";

    constexpr Vec3<float>   light_color     (1.0f, 0.0f, 0.0f);
    constexpr float         intensity       (100.0f);
    constexpr float         outer_radius    (50.0f);
    constexpr float         inner_radius    (40.0f);
    constexpr Vec3<float>   light_pos       (25.0f, -45.0f, -30.0f);
    constexpr Vec3<float>   dir_light       (1.0f, 0.0f, 0.0f);

    AABB raabb;

    SpotLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);
    light_builder.setOuterRadius(outer_radius);
    light_builder.setInnerRadius(inner_radius);
    light_builder.setDirection(dir_light);

    PtrSpotLight ptr_point_light = light_builder.buildPtr();

    SpotLightNode node1;
    SpotLightNode node2(name2, light_builder);

    constexpr Matrix4x4<float> rm;

    EXPECT_FALSE(node1.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldAABBIsCurrent())    << "При инициализации мировой ограничивающий объём должен быть не актуальным." << endl;

    EXPECT_FALSE(node1.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;
    EXPECT_FALSE(node2.worldTransformIsCurrent())   << "При инициализации мировое преобразование должно быть не актуальным." << endl;

    EXPECT_EQ(raabb, node1.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl; 
    EXPECT_EQ(raabb, node2.getWorldAABB())  << "Не правильная инициализация мирового ограничивающего объёма." << endl;

    EXPECT_EQ(nullptr, node1.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;
    EXPECT_EQ(nullptr, node2.getParent())   << "При инициализации у объекта появился указатель на родителя (его не должно быть)." << endl;

    EXPECT_TRUE(node1.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;
    EXPECT_TRUE(node2.getChildren().empty())    << "При инициализирование появились дочерние узлы." << endl;

    EXPECT_EQ(rm, node1.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;
    EXPECT_EQ(rm, node2.getWorldTransform().getMatrix())    << "Не правильное инициализирование мирового преобразования." << endl;

    EXPECT_EQ(rm, node1.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;
    EXPECT_EQ(rm, node2.getLocalTransform().getMatrix())    << "Не правильное инициализирование локального преобразования." << endl;

    EXPECT_EQ(name1, node1.getName())   << "Не правильное инициализирование имени." << endl;
    EXPECT_EQ(name2, node2.getName())   << "Не правильное инициализирование имени." << endl;
    
    EXPECT_EQ(nullptr, node1.getLight())            << "Не правильное инициализирование указателя на источник света." << endl;
    EXPECT_NE(nullptr, node2.getLight())            << "Не правильное инициализирование указателя на источник света." << endl;

    EXPECT_EQ(light_pos, node2.getLight()->getPosition())          << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(intensity, node2.getLight()->getIntensity())         << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(light_color, node2.getLight()->getColor())           << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(dir_light, node2.getLight()->getDirection())         << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(outer_radius, node2.getLight()->getOuterRadius())    << "Не правильное инициализирование источника света." << endl;
    EXPECT_EQ(inner_radius, node2.getLight()->getInnerRadius())    << "Не правильное инициализирование источника света." << endl;
}

TEST(SceneGraphSpotLightNode, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));

    constexpr Vec3<float>   light_color     (1.0f, 0.0f, 0.0f);
    constexpr float         intensity       (100.0f);
    constexpr float         outer_radius    (50.0f);
    constexpr float         inner_radius    (40.0f);
    constexpr Vec3<float>   light_pos       (25.0f, -45.0f, -30.0f);
    constexpr Vec3<float>   dir_light       (1.0f, 0.0f, 0.0f);

    const int init_value = rand();
    
    SpotLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);
    light_builder.setOuterRadius(outer_radius);
    light_builder.setInnerRadius(inner_radius);
    light_builder.setDirection(dir_light);

    PtrSpotLight ptr_point_light = light_builder.buildPtr();

    SpotLightNode      node1;
    PtrSpotLightNode   node2 = SpotLightNode::make();

    node1.setLight(light_builder);
    node2->setLight(ptr_point_light);

    EXPECT_EQ(light_pos, node1.getLight()->getPosition())   << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(intensity, node1.getLight()->getIntensity())  << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(light_color, node1.getLight()->getColor())    << "Ошибка в методе setLight(...)." << endl;

    EXPECT_EQ(light_pos, node2->getLight()->getPosition())  << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(intensity, node2->getLight()->getIntensity()) << "Ошибка в методе setLight(...)." << endl;
    EXPECT_EQ(light_color, node2->getLight()->getColor())   << "Ошибка в методе setLight(...)." << endl;

    EXPECT_FALSE(node1.hasSpotLightNodeModifier<TestSpotLightNodeModifier>()) << "Не правильное инициализирование модификаторов." << endl;

    {
        TestSpotLightNodeModifier* test_spot_light_node_modifier = new TestSpotLightNodeModifier;

        test_spot_light_node_modifier->i = init_value;
        node1.addSpotLightNodeModifier(test_spot_light_node_modifier);
    }

    EXPECT_TRUE(node1.hasSpotLightNodeModifier<TestSpotLightNodeModifier>())              << "Ошибка в методе addPointLightNodeModifier(...)." << endl;
    EXPECT_EQ(init_value, node1.getSpotLightNodeModifier<TestSpotLightNodeModifier>().i)  << "Ошибка в методе addPointLightNodeModifier(...)." << endl;
}

TEST(SceneGraphSpotLightNode, UpdateTest)
{
    Transform t1 = Transform::translate(Vec3<float>(-5.0f, 0.0f, 0.0f));
    Transform t2 = Transform::rotateX(60.0f);
    Transform t3 = Transform::rotateY(90.0f);

    AABB bbox1(
        Vec3<float>(0.5f),
        Vec3<float>(1.0f)
    );

    AABB bbox2(
        Vec3<float>(0.5f),
        Vec3<float>(-1.0f)
    );

    AABB bbox3(
        Vec3<float>(1.0f),
        Vec3<float>(-1.0f)
    );
    
    constexpr Vec3<float>   light_color     (1.0f, 0.0f, 0.0f);
    constexpr float         intensity       (100.0f);
    constexpr float         outer_radius    (50.0f);
    constexpr float         inner_radius    (40.0f);
    constexpr Vec3<float>   light_pos       (25.0f, -45.0f, -30.0f);
    constexpr Vec3<float>   dir_light       (1.0f, 0.0f, 0.0f);
    
    SpotLight::Builder light_builder;

    light_builder.setColor(light_color);
    light_builder.setIntensity(intensity);
    light_builder.setPosition(light_pos);
    light_builder.setOuterRadius(outer_radius);
    light_builder.setInnerRadius(inner_radius);
    light_builder.setDirection(dir_light);
    
    Scene scene;

    PtrSpotLightNode node1 = scene.makeSpotLight(light_builder, "Node 1");
    PtrSpotLightNode node2 = scene.makeSpotLight(light_builder, "Node 2");
    PtrSpotLightNode node3 = scene.makeSpotLight(light_builder, "Node 3");
    PtrSpotLightNode node4 = scene.makeSpotLight(light_builder, "Node 4");
    PtrSpotLightNode node5 = scene.makeSpotLight(light_builder, "Node 5");
    
    node1->addChild(node2);
    node1->addChild(node3);
    node2->addChild(node4);
    node3->addChild(node5);

    node1->setLocalTransform(t1);
    node2->setLocalTransform(t2);
    node3->setLocalTransform(t3);

    node4->setWorldAABB(bbox1);
    node5->setWorldAABB(bbox2);

    node1->update(0.2f, Transform());

    EXPECT_EQ(t1.getMatrix(), node1->getLocalTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node2->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(t1.getMatrix(), node3->getWorldTransform().getMatrix())           << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t2).getMatrix(), node4->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ((t1 * t3).getMatrix(), node5->getWorldTransform().getMatrix())    << "Не правильно работает метод update(...)." << endl;

    EXPECT_EQ(bbox3, node1->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node2->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node3->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox1, node4->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
    EXPECT_EQ(bbox2, node5->getWorldAABB()) << "Не правильно работает метод update(...)." << endl;
}
