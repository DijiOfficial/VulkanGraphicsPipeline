#pragma once
#include <vulkan/vulkan_core.h>

class VulkanShader;
class GraphicsPipeline final
{
public:
    GraphicsPipeline() = default;
    ~GraphicsPipeline() noexcept = default;

    GraphicsPipeline(const GraphicsPipeline& other) = delete;
    GraphicsPipeline(GraphicsPipeline&& other) = delete;
    GraphicsPipeline& operator=(const GraphicsPipeline& other) = delete;
    GraphicsPipeline& operator=(GraphicsPipeline&& other) = delete;

    void CreateGraphicsPipeline(VulkanShader& shader, const VkRenderPass& renderPass, const VkPipelineVertexInputStateCreateInfo& pipelineVertexInputStateCreateInfo);
    void Destroy();

    VkPipeline& GetGraphicsPipeline() { return m_GraphicsPipeline; }
    VkPipelineLayout GetPipelineLayout() const { return m_PipelineLayout; }

private:
    VkPipelineLayout m_PipelineLayout;
    VkPipeline m_GraphicsPipeline;
};