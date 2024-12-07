//
// Created by user on 11/28/2024.
//
#pragma once
#include <vulkan/vulkan_core.h>
#include "Instance.h"

namespace ScarlattiCore {
    class Device;
    struct SwapChainSupportDetails;
    class SwapChain {
    public:
        SwapChain(Device _device, GLFWwindow* _window);
        VkSwapchainKHR getHandler();
        std::vector<VkImage> getImages();
    private:
        VkSwapchainKHR handler{VK_NULL_HANDLE};
        SwapChainSupportDetails swapChainSupport;
        Device &device;
        std::vector<VkImage> swapChainImages;
        GLFWwindow* window;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
};

} // ScarlattiCore
