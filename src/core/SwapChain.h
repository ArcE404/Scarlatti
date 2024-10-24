//
// Created by user on 23/10/2024.
//

#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H

#include "../Common.h"

namespace Scarlatti {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

class SwapChain {
    private:
    static SwapChainSupportDetails* swapChainDetails;

    public:
    static SwapChainSupportDetails* querySwapChainSupport(VkPhysicalDevice);
};

} // Scarlatti

#endif //SWAPCHAIN_H
