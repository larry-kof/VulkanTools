//
//  VksFramebuffer.cpp
//  Vulkan
//
//  Created by larry-kof on 2020/8/14.
//  Copyright © 2020 larry. All rights reserved.
//

#include "VksFramebuffer.hpp"
#include "VksCommand.hpp"
#include <array>
#include <iostream>

VksFramebuffer::VksFramebuffer( const std::shared_ptr<VksTexture>& colorTexture,
                               const std::shared_ptr<VksRenderPass>& renderPass )
    :VkEngine(), m_framebuffer(VK_NULL_HANDLE), m_colorTexture( colorTexture )
    ,m_commandBuffer( VK_NULL_HANDLE )
{
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.attachmentCount = 1;
    const VkImageView& colorView = colorTexture->getImageView();
    framebufferInfo.pAttachments = &colorView;
    framebufferInfo.renderPass = renderPass->getVkRenderPass();
    framebufferInfo.layers = 1;
    framebufferInfo.width = colorTexture->getWidth();
    framebufferInfo.height = colorTexture->getHeight();
    m_width = framebufferInfo.width;
    m_height = framebufferInfo.height;
    
    m_rendePass = renderPass;
    
    VK_CHECK( vkCreateFramebuffer(m_logicDevice, &framebufferInfo, nullptr, &m_framebuffer) )
    m_commandBuffer = m_graphicCommand->createPrimaryBuffer();
}

VksFramebuffer::VksFramebuffer( const std::shared_ptr<VksTexture>& colorTexture, const std::shared_ptr<VksTexture>& depthStencilTexture, const std::shared_ptr<VksRenderPass>& renderPass)
    :VkEngine(), m_framebuffer(VK_NULL_HANDLE), m_colorTexture(colorTexture)
    ,m_depthStencilTexture( depthStencilTexture )
    ,m_commandBuffer( VK_NULL_HANDLE )
{
    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.attachmentCount = 2;
    const VkImageView& colorView = colorTexture->getImageView();
    const VkImageView& depthView = depthStencilTexture->getImageView();
    
    std::array<VkImageView, 2> attachments = {
        colorView,
        depthView
    };

    framebufferInfo.pAttachments = attachments.data();
    framebufferInfo.renderPass = renderPass->getVkRenderPass();
    framebufferInfo.layers = 1;
    framebufferInfo.width = colorTexture->getWidth();
    framebufferInfo.height = colorTexture->getHeight();
    m_width = framebufferInfo.width;
    m_height = framebufferInfo.height;
    
    m_rendePass = renderPass;

    VK_CHECK( vkCreateFramebuffer(m_logicDevice, &framebufferInfo, nullptr, &m_framebuffer) )
    m_commandBuffer = m_graphicCommand->createPrimaryBuffer();
}

void VksFramebuffer::useGraphicPipeline(const std::shared_ptr<VksGraphicPipeline> &graphicPipeline)
{
    m_graphicPipeline = graphicPipeline;
    m_graphicPipeline->addComponent<VksRenderPass>( m_rendePass.get() );
    m_graphicPipeline->__createGraphicPipeline();
    vkCmdBindPipeline(m_commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicPipeline->getVkGraphicPipele());
}

void VksFramebuffer::bind()
{
    m_graphicCommand->beginRenderPass(m_commandBuffer, shared_from_this());
}

void VksFramebuffer::unBind()
{
    m_graphicCommand->endRenderPass(m_commandBuffer);
}

void VksFramebuffer::bindUniformSets(int setsIndex)
{
    auto shader = m_graphicPipeline->m_weakShader.lock();
    if( !shader ) return;
    m_graphicCommand->bindUniformSet(m_commandBuffer, shader, shader->getDescriptorSet( setsIndex ));
}

void VksFramebuffer::bindIndexBuffer(const std::shared_ptr<VksBuffer> &indexBuffer)
{
    m_graphicCommand->bindIndexBuffer(m_commandBuffer, indexBuffer);
}

void VksFramebuffer::bindVertexBuffer(const std::shared_ptr<VksBuffer> &vertexBuffer)
{
    m_graphicCommand->bindVertexBuffer(m_commandBuffer, vertexBuffer);
}

void VksFramebuffer::draw(int vertexCount)
{
    m_graphicCommand->draw(m_commandBuffer, vertexCount);
}

void VksFramebuffer::drawIndexed(int indexCount)
{
    m_graphicCommand->drawIndexed(m_commandBuffer, indexCount);
}

VksFramebuffer::~VksFramebuffer()
{
    if( m_framebuffer != VK_NULL_HANDLE )
    {
        vkDestroyFramebuffer(m_logicDevice, m_framebuffer, nullptr);
    }
    
    if( m_commandBuffer != VK_NULL_HANDLE )
    {
        vkFreeCommandBuffers(m_logicDevice, m_graphicCommand->getCommandPool(), 1, &m_commandBuffer);
    }
}
