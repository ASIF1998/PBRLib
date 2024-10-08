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
    class ImageView;

    using PtrAttachments = std::shared_ptr<std::vector<ImageView>>;
}

namespace pbrlib
{

    class Framebuffer
    {
    public:
        class Builder
        {
        public:
            Builder();

            Builder(Builder&&)      = delete;
            Builder(const Builder&) = delete;

            Builder& operator = (Builder&&)         = delete;
            Builder& operator = (const Builder&)    = delete;

            /**
             * @brief Метод устанавливающий список отображения.
             * 
             * @param ptr_swapchain указатель на список отображения.
            */
            void setSwapchain(std::shared_ptr<const Swapchain> ptr_swapchain);

            /**
             * @brief Метод устанавливающий индекс вида изображения внутри списка отображения.
             * 
             * @param swapchain_attachment_index индекс вида изображения внутри списка отображения.
            */
            void setSwapchainAttachmentIndex(uint32_t swapchain_attachment_index);

            /**
             * @brief 
             *      Метод устанавливающий проход рендеринга.
             * 
             * @param ptr_render_pass указатель на проход рендеринга.
            */
            void setRenderPass(std::shared_ptr<const RenderPass> ptr_render_pass);

            /**
             * @brief Метод устанавливающий виды изображений.
             * 
             * @param ptr_attachments  указатель на виды изображений (например глубина-трафарет).
            */
            void setAttachments(const PtrAttachments& ptr_attachments);

            void setWidth(uint32_t width)           noexcept;
            void setHeight(uint32_t height)         noexcept;
            void setNumLayers(uint32_t num_layers)  noexcept;

            Framebuffer                     build()     const;
            std::unique_ptr<Framebuffer>    buildPtr()  const;

        private:
            std::shared_ptr<const Swapchain>    _ptr_swapchain;
            std::shared_ptr<const RenderPass>   _ptr_render_pass;
            PtrAttachments                      _ptr_attachments;
            uint32_t                            _swapchain_attachment_index;
            uint32_t                            _width;
            uint32_t                            _height;
            uint32_t                            _layers;
        };

    public:
        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param ptr_render_pass           указатель на проход рендеринга.
         * @param attachments               иные виды изображений (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        Framebuffer(
            std::shared_ptr<const Swapchain>    ptr_swapchain,
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            PtrAttachments&&                    attachments,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param ptr_render_pass           указатель на проход рендеринга.
         * @param attachments               иные виды изображений (например глубина-трафарет), 
         *                                  которые, при создании фреймбуфера, будут идти после 
         *                                  видов изображения в списке отображения.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */
        Framebuffer(
            std::shared_ptr<const Swapchain>    ptr_swapchain,
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            const PtrAttachments&               attachments,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param ptr_swapchain             указатель на список отображения.
         * @param swapchain_attachment_indx индекс вида изображения внутри списка отображения.
         * @param ptr_render_pass           указатель на проход рендеринга.
         * @param width                     ширина фреймбуфера.
         * @param height                    высота фреймбуфера.
         * @param layers                    количество слоёв фреймбуфера.
        */  
        Framebuffer(
            std::shared_ptr<const Swapchain>    ptr_swapchain, 
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments       виды изображений.
         * @param ptr_render_pass   указатель на проход рендеринга.
         * @param width             ширина фреймбуфера.
         * @param height            высота фреймбуфера.
         * @param layers            количество слоёв фреймбуфера.
        */  
        Framebuffer(
            PtrAttachments&&                    attachments,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Конструктор.
         * 
         * @param attachments       виды изображений.
         * @param ptr_render_pass   указатель на проход рендеринга.
         * @param width             ширина фреймбуфера.
         * @param height            высота фреймбуфера.
         * @param layers            количество слоёв фреймбуфера.
        */  
        Framebuffer(
            const PtrAttachments&               attachments,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        Framebuffer(Framebuffer&& framebuffer);
        Framebuffer(const Framebuffer& framebuffer) = delete;

        ~Framebuffer();

        Framebuffer& operator = (Framebuffer&&)         = delete;
        Framebuffer& operator = (const Framebuffer&)    = delete;

        bool isUsedSwapchain() const noexcept;

        std::shared_ptr<const Swapchain>    getSwapchain()                  const noexcept;
        std::shared_ptr<const RenderPass>   getRenderPass()                 const noexcept;
        PtrAttachments&                     getAttachments()                noexcept;
        const PtrAttachments&               getAttachments()                const noexcept;
        const ImageView&                    getSwapchainAttachment()        const noexcept;
        uint32_t                            getSwapchainAttachmentIndex()   const noexcept;
        const VkFramebuffer&                getFramebufferHandle()          const noexcept;
        uint32_t                            getWidth()                      const noexcept;
        uint32_t                            getHeight()                     const noexcept;
        uint32_t                            getNumLayers()                  const noexcept;
        const Device*                       getDevice()                     const noexcept;

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
        static std::unique_ptr<Framebuffer> make(
            std::shared_ptr<const Swapchain>    ptr_swapchain,
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            PtrAttachments&&                    attachments,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
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
        static std::unique_ptr<Framebuffer> make(
            std::shared_ptr<const Swapchain>    ptr_swapchain,
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            const PtrAttachments&               attachments,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
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
        static std::unique_ptr<Framebuffer> make(
            std::shared_ptr<const Swapchain>    ptr_swapchain, 
            uint32_t                            swapchain_attachment_indx,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        static std::unique_ptr<Framebuffer> make(
            PtrAttachments&&                    attachments,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

        /**
         * @brief Статический метод для создания указателя на Framebuffer.
         * 
         * @param attachments   виды изображений.
         * @param width         ширина фреймбуфера.
         * @param height        высота фреймбуфера.
         * @param layers        количество слоёв фреймбуфера.
        */  
        static std::unique_ptr<Framebuffer> make(
            const PtrAttachments&               attachments,
            std::shared_ptr<const RenderPass>   ptr_render_pass,
            uint32_t                            width,
            uint32_t                            height,
            uint32_t                            layers
        );

    private:
        void _create();

    private:  
        std::shared_ptr<const Swapchain>    _ptr_swapchain;
        std::shared_ptr<const RenderPass>   _ptr_render_pass;
        uint32_t                            _swapchain_attachment_indx;
        PtrAttachments                      _attachments;
        VkFramebuffer                       _framebuffer_handle;
        uint32_t                            _width;
        uint32_t                            _height;
        uint32_t                            _layers;
    };
}

#endif /* Framebuffer_hpp */
