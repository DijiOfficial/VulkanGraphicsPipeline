#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>

struct SwapChainImageView final
{
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    std::vector<VkImageView> swapChainImageViews;

    void CreateImageViews();

    //maybe no need for static if I can pass it to texture
    static VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);
};