#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>

class DepthBuffer final
{
public:
    DepthBuffer() = default;
    ~DepthBuffer() = default;

    void Destroy();

    DepthBuffer(const DepthBuffer& other) = delete;
    DepthBuffer(DepthBuffer&& other) = delete;
    DepthBuffer& operator=(const DepthBuffer& other) = delete;
    DepthBuffer& operator=(DepthBuffer&& other) = delete;

    void CreateDepthResources(const VkExtent2D& swapChainExtent);
    
    //VkImage GetDepthImage() { return depthImage; }
    VkImageView GetDepthImageView() const { return m_DepthImageView; }
    VkFormat FindDepthFormat();

private:
    VkImage m_DepthImage;
    VkDeviceMemory m_DepthImageMemory;
    VkImageView m_DepthImageView;

    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    bool HasStencilComponent(VkFormat format);
};