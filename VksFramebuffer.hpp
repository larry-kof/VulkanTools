//
//  VksFramebuffer.hpp
//  Vulkan
//
//  Created by larry-kof on 2020/8/14.
//  Copyright © 2020 larry. All rights reserved.
//

#ifndef VksFramebuffer_hpp
#define VksFramebuffer_hpp

#include <stdio.h>
#include "VkEngine.hpp"
#include "VksTexture.hpp"
#include "VksRenderPass.hpp"
#include "VksGraphicPipeline.hpp"

class VksFramebuffer : protected VkEngine, public std::enable_shared_from_this<VksFramebuffer>
{
public:
    VksFramebuffer( const std::shared_ptr<VksTexture>& colorTexture, const std::shared_ptr<VksRenderPass>& renderPass);
    VksFramebuffer( const std::shared_ptr<VksTexture>& colorTexture, const std::shared_ptr<VksTexture>& depthStencilTexture, const std::shared_ptr<VksRenderPass>& renderPass);
    
    void useGraphicPipeline( const std::shared_ptr<VksGraphicPipeline>& graphicPipeline );
    
    ~VksFramebuffer();
    
    VkFramebuffer getVkFramebuffer()
    {
        return m_framebuffer;
    }
    
    VkExtent2D getFramebufferSize()
    {
        VkExtent2D size = { m_width, m_height };
        return size;
    }
    
    const VkCommandBuffer getVkCommandBuffer()
    {
        return m_commandBuffer;
    }
    
    VkRenderPass getVkRenderPass()
    {
        return m_rendePass->getVkRenderPass();
    }
    
    void bind();
    void unBind();
    
    void bindUniformSets( int setsIndex );
    void bindVertexBuffer( const std::shared_ptr<VksBuffer>& vertexBuffer );
    void bindIndexBuffer( const std::shared_ptr<VksBuffer>& indexBuffer );
    void draw( int vertexCount );
    void drawIndexed( int indexCount );
    
private:
    VkFramebuffer m_framebuffer;
    
    std::shared_ptr<VksTexture> m_colorTexture;
    std::shared_ptr<VksTexture> m_depthStencilTexture;
    std::shared_ptr<VksRenderPass> m_rendePass;
    std::shared_ptr<VksGraphicPipeline> m_graphicPipeline;
    VkCommandBuffer m_commandBuffer;
    uint32_t m_width;
    uint32_t m_height;
};

#endif /* VksFramebuffer_hpp */
