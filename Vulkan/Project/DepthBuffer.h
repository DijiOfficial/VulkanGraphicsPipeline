//#pragma once
//#include <vulkan/vulkan_core.h>
//#include <vector>
//
//class DepthBuffer final
//{
//public:
//    DepthBuffer() = default;
//    ~DepthBuffer() = default;
//
//    DepthBuffer(const DepthBuffer& other) = delete;
//    DepthBuffer(DepthBuffer&& other) = delete;
//    DepthBuffer& operator=(const DepthBuffer& other) = delete;
//    DepthBuffer& operator=(DepthBuffer&& other) = delete;
//
//    void CreateDepthResources();
//    
//    //VkImage GetDepthImage() { return depthImage; }
//    //VkImageView GetDepthImageView() { return depthImageView; }
//    //void DestroyDepthResources();
//
//private:
//    VkImage m_DepthImage;
//    VkDeviceMemory m_DepthImageMemory;
//    VkImageView m_DepthImageView;
//
//    VkFormat FindDepthFormat();
//    VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
//    bool HasStencilComponent(VkFormat format);
//};