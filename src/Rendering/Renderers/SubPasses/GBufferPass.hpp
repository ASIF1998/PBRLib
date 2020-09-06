//
//  GBufferPass.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 20/06/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef GBufferPass_hpp
#define GBufferPass_hpp

#include <memory>

#include "../../../SceneGraph/Scene.hpp"

using namespace std;

namespace pbrlib
{
    class GraphicsPipeline;
    class Buffer;
    class DescriptorPool;
    class DescriptorSet;
    class Device;
    class RenderPass;
    class CommandBuffer;
    class Sampler;
    class GBufferPass;

    using PtrGraphicsPipeline   = shared_ptr<GraphicsPipeline>;
    using PtrBuffer             = shared_ptr<Buffer>;
    using PtrDescriptorPool     = shared_ptr<DescriptorPool>;
    using PtrDescriptorSet      = shared_ptr<DescriptorSet>;
    using PtrDevice             = shared_ptr<Device>;
    using PtrRenderPass         = shared_ptr<RenderPass>;
    using PtrCommandBuffer      = shared_ptr<CommandBuffer>;
    using PtrSampler            = shared_ptr<Sampler>;
    using PtrGBufferPass        = unique_ptr<GBufferPass>;

    /**
     * @class GBufferPass.
     * @brief Данный класс позволяет создавать g-buffer.
     * @details 
     *      Для работы с этим классом, проход рендера должен 
     *      содержать следующие подключения:
     *          1) для позиции (vec3) и металичности (float);
     *          2) для нормали (vec3) и шероховатости (float);
     *          3) для альбедо (vec3) и запечённого AO (float);
     *          4) для параметра анизотропности (float).
    */
    class GBufferPass
    {
    public:
        class Builder
        {
        public:
            Builder() = default;

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)         = delete;
            Builder& operator = (const Builder&)    = delete;

            void setDevice(const PtrDevice& ptr_device);
            void setDescriptorPool(const PtrDescriptorPool& ptr_descriptor_pool);
            void setRenderPass(const PtrRenderPass& ptr_render_pass);
            void setSubpassIndex(uint32_t subpass_index);
            void setDeviceMemoryIndex(uint32_t memory_index);
            void setDeviceQueueFamilyIndex(uint32_t queue_family_index);
            void windowSize(uint32_t width, uint32_t height);

            GBufferPass     build();
            PtrGBufferPass  buildPtr();

        private:
            PtrDevice           _ptr_device;
            PtrDescriptorPool   _ptr_descriptor_pool;
            PtrRenderPass       _ptr_render_pass;
            uint32_t            _subpass_index;
            uint32_t            _window_width;
            uint32_t            _window_height;
            uint32_t            _gpu_memory_index;
            uint32_t            _gpu_queue_family_index;
        };
        
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_device                указатель на логическое устройство.
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param ptr_render_pass           указатель на проход рендеринга.
         * @param subpass_index             индекс подпрохода.
         * @param gpu_memory_index          индекс памяти устройства (должен поддерживать отображение).
         * @param gpu_queue_family_index    индекс семейства очередей (семейство должно поддерживать графические операции).
         * @param window_width              ширина окна.
         * @param window_height             высота окна.
        */
        GBufferPass(
            const PtrDevice&            ptr_device, 
            const PtrDescriptorPool&    ptr_descriptor_pool,  
            const PtrRenderPass&        ptr_render_pass,
            uint32_t                    subpass_index,
            uint32_t                    gpu_memory_index,
            uint32_t                    gpu_queue_family_index,
            uint32_t                    window_width,
            uint32_t                    window_height
        );

        /**
         * @brief Метод, позволяющий запустить графический конвейер.
         * @details 
         *      Объект будет отображён в случае, если у него есть
         *      меш с материалом.
         * 
         * @param projection            проекция.
         * @param view                  преобразование в пространство вида (камеры).
         * @param drawable_objects      отображаемые объекты.
         * @param ptr_command_buffer    указатель на командный буфер.
         * @param ptr_sampler           указатель на сэмплер.
        */
        void draw(
            const Transform&            projection,
            const Transform&            view,
            const Scene::VisibleList&   drawable_objects, 
            const PtrCommandBuffer&     ptr_command_buffer,
            const PtrSampler&           ptr_sampler
        );

        PtrGraphicsPipeline&         getPipeline()       noexcept;
        const PtrGraphicsPipeline&   getPipeline()       const noexcept;
        PtrDescriptorSet&            getDescriptorSet()  noexcept;
        const PtrDescriptorSet&      getDescriptorSet()  const noexcept;

        /**
         * @brief Метод, позволяющий указатель на объект типа GBufferPass.
         * 
         * @param ptr_device                указатель на логическое устройство.
         * @param ptr_descriptor_pool       указатель на пул дескрипторов.
         * @param ptr_render_pass           указатель на проход рендеринга.
         * @param subpass_index             индекс подпрохода.
         * @param gpu_memory_index          индекс памяти устройства (должен поддерживать отображение).
         * @param gpu_queue_family_index    индекс семейства очередей (семейство должно поддерживать графические операции).
         * @param window_width              ширина окна.
         * @param window_height             высота окна.s
         * @return Указатель на GBufferPass.
        */
        static PtrGBufferPass make(
            const PtrDevice&            ptr_device, 
            const PtrDescriptorPool&    ptr_descriptor_pool,  
            const PtrRenderPass&        ptr_render_pass,
            uint32_t                    subpass_index,
            uint32_t                    gpu_memory_index,
            uint32_t                    gpu_queue_family_index,
            uint32_t                    window_width,
            uint32_t                    window_height
        );

    private:
        PtrGraphicsPipeline _ptr_pipeline;
        PtrDescriptorSet    _ptr_descriptor_set;
        PtrBuffer           _ptr_uniform_matrices_data_buffer;
        PtrBuffer           _ptr_uniform_material_data_buffer;
    };
}

#endif /* GBufferPass_hpp */
