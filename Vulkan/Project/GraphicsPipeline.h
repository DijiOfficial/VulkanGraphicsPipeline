#pragma once
#include <vulkan/vulkan_core.h>

class VulkanShader;
class GraphicsPipeline final
{
public:
    //static VkPipelineLayout GetPipelineLayout() { return m_PipelineLayout; }

    //void CreateGraphicsPipeline(const VkRenderPass& renderPass, Shader& gradientShader, VkPipelineVertexInputStateCreateInfo pipelineVerInputStateCreateInfo, bool enableDepthBuffering = true);
    void CreateGraphicsPipeline(VulkanShader& shader, const VkRenderPass& renderPass);
    void Destroy();


    VkPipeline& GetGraphicsPipeline() { return m_GraphicsPipeline; }
    //static void CreatePipelineLayout();
private:

    //static VkPipelineLayout m_PipelineLayout;
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;
};