//
// Created by user on 23/10/2024.
//

#include "SwapChain.h"

namespace Scarlatti {
    SwapChainSupportDetails* SwapChain::querySwapChainSupport(VkPhysicalDevice device) {
        if(swapChainDetails == nullptr) {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapChainDetails->capabilities);

            // surface formats
            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

            if (formatCount != 0) {
                swapChainDetails->formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapChainDetails->formats.data());
            }

            // presentation modes
            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

            if (presentModeCount != 0) {
                swapChainDetails->presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapChainDetails->presentModes.data());
            }

            return swapChainDetails;
        }

        return swapChainDetails;
    }
} // Scarlatti