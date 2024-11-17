//
// Created by user on 05/11/2024.
//
#pragma once
#include "PhysicalDevice.h"
#include <queue>
#include "../resources/DeviceResourceCache.h"

namespace ScarlattiCore {
class Device {
public:
    Device(PhysicalDevice                        &gpu,
           VkSurfaceKHR                           surface,
           std::vector<const char *> requested_extensions = {});

    uint32_t getQueueFamilyIndex(VkQueueFlagBits queue_flag_bits);
    VkDevice getHandler();
    VkQueue getGraphicsQueue() const;
    VkQueue getPresentQueue() const;
private:
    const PhysicalDevice &gpu;
    VkSurfaceKHR surface{VK_NULL_HANDLE};
    std::vector<const char *> enabledExtensions{};
    ScarlattiResources::DeviceResourceCache resourceCache;
    VkDevice handler{VK_NULL_HANDLE};
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

} // ScarlattiCore
