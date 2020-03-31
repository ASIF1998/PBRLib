//
//  Framebuffer.hpp
//  PBRLib
//
//  Created by Асиф Мамедов on 18/02/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef Framebuffer_hpp
#define Framebuffer_hpp

#include "RenderPass.hpp"
#include "Swapchain.hpp"

namespace pbrlib
{
    class Framebuffer;
    class ImageView;

    using PtrAttachments = shared_ptr<vector<ImageView>>;
    using PtrFramebuffer = shared_ptr<Framebuffer>;

    class Framebuffer
    {
    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        Framebuffer(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            PtrAttachments&&        attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        Framebuffer(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            const PtrAttachments&   attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */  
        Framebuffer(
            const PtrSwapchain&     ptr_swapchain, 
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        Framebuffer(
            PtrAttachments&&        attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        Framebuffer(
            const PtrAttachments&   attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        Framebuffer(Framebuffer&& framebuffer);
        Framebuffer(const Framebuffer& framebuffer) = delete;

        ~Framebuffer();

        Framebuffer& operator = (Framebuffer&&)         = delete;
        Framebuffer& operator = (const Framebuffer&)    = delete;

        bool isUsedSwapchain() const noexcept;

        PtrSwapchain&            getSwapchain()                  noexcept;
        const PtrSwapchain&      getSwapchain()                  const noexcept;
        PtrRenderPass&           getRenderPass()                 noexcept;
        const PtrRenderPass&     getRenderPass()                 const noexcept;
        PtrAttachments&          getAttachments()                noexcept;
        const PtrAttachments&    getAttachments()                const noexcept;
        ImageView&               getSwapchainAttachment()        noexcept;
        const ImageView&         getSwapchainAttachment()        const noexcept;
        uint32_t                 getSwapchainAttachmentIndex()   const noexcept;
        const VkFramebuffer&     getFramebufferHandle()          const noexcept;
        uint32_t                 getWidth()                      const noexcept;
        uint32_t                 getHeight()                     const noexcept;
        uint32_t                 getNumLayers()                  const noexcept;
        PtrDevice&               getDevice()                     noexcept;
        const PtrDevice&         getDevice()                     const noexcept;

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            PtrAttachments&&        attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param attachments               иные виды изображения (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain,
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            const PtrAttachments&   attachments,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */  
        static PtrFramebuffer make(
            const PtrSwapchain&     ptr_swapchain, 
            uint32_t                swapchain_attachment_indx,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        static PtrFramebuffer make(
            PtrAttachments&&        attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        static PtrFramebuffer make(
            const PtrAttachments&   attachments,
            const PtrRenderPass&    ptr_render_pass,
            uint32_t                width,
            uint32_t                height,
            uint32_t                layers
        );

    private:
        void _create();

    private:  
        PtrSwapchain    _ptr_swapchain;
        PtrRenderPass   _ptr_render_pass;
        uint32_t        _swapchain_attachment_indx;
        PtrAttachments  _attachments;
        VkFramebuffer   _framebuffer_handle;
        uint32_t        _width;
        uint32_t        _height;
        uint32_t        _layers;
    };
}

#endif /* Framebuffer_hpp */
