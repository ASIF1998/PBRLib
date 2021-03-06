//
//  MeshTests.cpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 29/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#include <vector>

#include <gtest/gtest.h>

#include "../../../src/Rendering/Geometry/Mesh.hpp"
#include "../../../src/Rendering/Geometry/MeshManager.hpp"

#include "../../../src/math/vec3.hpp"
#include "../../../src/math/vec2.hpp"

#include "../../../src/Rendering/VulkanWrapper/Instance.hpp"
#include "../../../src/Rendering/VulkanWrapper/PhysicalDevice.hpp"
#include "../../../src/Rendering/VulkanWrapper/Device.hpp"

#include "../../../src/Rendering/VulkanWrapper/Buffer.hpp"

using namespace testing;
using namespace pbrlib;
using namespace pbrlib::math;
using namespace std;

TEST(RenderingGeometryMesh, GettersAndSetters)
{
    srand(static_cast<unsigned>(time(nullptr)));
    
    PtrInstance ptr_instance = Instance::make(
        "RenderingResourceMesh::GettersAndSetters", 
        VK_MAKE_VERSION(0, 0, 1)
    );

    PtrPhysicalDevice gpu = ptr_instance->getPhysicalDevice(
        VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU    |
        VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU  |
        VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU
    );

    float queue_priority = 1.0f;

    vector<VkDeviceQueueCreateInfo> device_queue_create_info;

    device_queue_create_info.push_back(
        VkDeviceQueueCreateInfo {
            .sType              = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex   = 0,
            .queueCount         = 1,
            .pQueuePriorities   = &queue_priority
        }
    );

    PtrDevice ptr_device = gpu->makeDevice(ptr_instance, device_queue_create_info);

    constexpr size_t num_vertex = 200;
    constexpr size_t num_indices = 152;

    constexpr Vec3<float>   pnt_r   (0.00334f, 0.54540f, 0.4567f);
    constexpr Vec2<float>   uv_r    (0.65340f, 0.67003f);
    constexpr uint32_t      index_r (1332);

    vector<Mesh::VertexAttrib>  vertices;
    vector<uint32_t>            indices;

    vertices.reserve(num_vertex);
    indices.reserve(num_indices);

    for(size_t i{0}; i < num_vertex; i++) {
        vertices.push_back(Mesh::VertexAttrib{
            .position = Vec3<float> (
                static_cast<float>(rand()),
                static_cast<float>(rand()),
                static_cast<float>(rand())
            ),

            .uv = Vec2<float> (
                static_cast<float>(rand()),
                static_cast<float>(rand())
            ),

            .normal = Vec3<float> (
                static_cast<float>(rand()),
                static_cast<float>(rand()),
                static_cast<float>(rand())
            ),

            .tangent = Vec3<float> (
                static_cast<float>(rand()),
                static_cast<float>(rand()),
                static_cast<float>(rand())
            ),
        });
    }

    for (uint32_t i{0}; i < num_indices; i++) {
        indices.push_back(i * 2);
    }

    Buffer::BuilderWithData<Mesh::VertexAttrib> build_vertex_buffer;
    Buffer::BuilderWithData<uint32_t>           build_index_buffer;

    build_vertex_buffer.setUsage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    build_vertex_buffer.setMemoryTypeIndex(gpu->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
    build_vertex_buffer.setDevice(ptr_device);
    build_vertex_buffer.setData(vertices);
    build_vertex_buffer.addQueueFamily(0);

    build_index_buffer.setUsage(VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    build_index_buffer.setMemoryTypeIndex(gpu->memory.getMemoryType(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT));
    build_index_buffer.setDevice(ptr_device);
    build_index_buffer.setData(indices);
    build_index_buffer.addQueueFamily(0);

    PtrBuffer ptr_vertex_buffer = build_vertex_buffer.buildPtr();
    PtrBuffer ptr_index_buffer  = build_index_buffer.buildPtr();

    Mesh mesh1;
    Mesh mesh2;
    
    mesh1.setVertexBuffer(ptr_vertex_buffer);
    mesh1.setIndexBuffer(ptr_index_buffer);
    mesh1.setNumVertices(num_vertex / 2);
    mesh1.setNumIndices(num_indices / 2);
    mesh1.setIndexBufferOffset(0);
    mesh1.setVertexBufferOffset(0);
    
    mesh2.setVertexBuffer(ptr_vertex_buffer);
    mesh2.setIndexBuffer(ptr_index_buffer);
    mesh2.setNumVertices(num_vertex / 2);
    mesh2.setNumIndices(num_indices / 2);
    mesh2.setIndexBufferOffset(num_indices / 2);
    mesh2.setVertexBufferOffset(num_vertex / 2);
    
    mesh1.mapVertexAttribBuffer();

    for (size_t i{0}; i < num_vertex / 2; i++) {
        EXPECT_EQ(vertices[i].position, mesh1.getAttribute<MeshAttribute::Position>(i));
        EXPECT_EQ(vertices[i].uv, mesh1.getAttribute<MeshAttribute::UV>(i));
        EXPECT_EQ(vertices[i].normal, mesh1.getAttribute<MeshAttribute::Normal>(i));
        EXPECT_EQ(vertices[i].tangent, mesh1.getAttribute<MeshAttribute::Tangent>(i));
    }

    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();

    for (size_t i{0}; i < num_indices / 2; i++) {
        EXPECT_EQ(indices[i], mesh1.getIndex(i));
    }

    mesh1.unmapIndexBuffer();

    mesh2.mapVertexAttribBuffer();

    for (size_t i{mesh1.getVertexBufferOffset()}; i < mesh1.getNumVertices(); i++) {
        EXPECT_EQ(vertices[i].position, mesh2.getAttribute<MeshAttribute::Position>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].uv, mesh2.getAttribute<MeshAttribute::UV>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].normal, mesh2.getAttribute<MeshAttribute::Normal>(i - (num_vertex / 2)));
        EXPECT_EQ(vertices[i].tangent, mesh2.getAttribute<MeshAttribute::Tangent>(i - (num_vertex / 2)));
    }

    mesh2.unmapVertexAttribBuffer();

    mesh2.mapIndexBuffer();

    for (size_t i{num_indices / 2}; i < num_indices; i++) {
        EXPECT_EQ(indices[i], mesh2.getIndex(i - (num_indices / 2)));
    }

    mesh2.unmapIndexBuffer();

    mesh1.mapVertexAttribBuffer();
    mesh2.mapVertexAttribBuffer();

    mesh1.getAttribute<MeshAttribute::Position>(13) = pnt_r;
    mesh1.getAttribute<MeshAttribute::UV>(13)       = uv_r;
    mesh1.getAttribute<MeshAttribute::Normal>(13)   = pnt_r;
    mesh1.getAttribute<MeshAttribute::Tangent>(13)  = pnt_r;

    mesh2.getAttribute<MeshAttribute::Position>(13) = pnt_r;
    mesh2.getAttribute<MeshAttribute::UV>(13)       = uv_r;
    mesh2.getAttribute<MeshAttribute::Normal>(13)   = pnt_r;
    mesh2.getAttribute<MeshAttribute::Tangent>(13)  = pnt_r;

    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Position>(13), pnt_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::UV>(13), uv_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    EXPECT_EQ(mesh1.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Position>(13), pnt_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::UV>(13), uv_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Normal>(13), pnt_r);
    EXPECT_EQ(mesh2.getAttribute<MeshAttribute::Tangent>(13), pnt_r);

    mesh2.unmapVertexAttribBuffer();
    mesh1.unmapVertexAttribBuffer();

    mesh1.mapIndexBuffer();
    mesh2.mapIndexBuffer();

    mesh1.setIndex(23, index_r);
    mesh2.setIndex(23, index_r);

    EXPECT_EQ(index_r, mesh1.getIndex(23));
    EXPECT_EQ(index_r, mesh2.getIndex(23));

    mesh2.unmapIndexBuffer();
    mesh1.unmapIndexBuffer();
}
