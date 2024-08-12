#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>

struct SwapChainImageView final
{
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    std::vector<VkImageView> swapChainImageViews;

    void CreateImageViews();

    //static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT);
};