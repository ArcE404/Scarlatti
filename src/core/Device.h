//
// Created by user on 05/11/2024.
//
#pragma once
#include "PhysicalDevice.h"
#include <queue>

namespace ScarlattiCore {
class Device {
public:
    Device(PhysicalDevice                        &gpu,
           VkSurfaceKHR                           &surface,
           std::vector<const char *> requested_extensions = {},
           const std::vector<const char *>& requested_layers = {});

    ~Device();

    uint32_t getQueueFamilyIndex(VkQueueFlagBits queue_flag_bits);
    VkDevice getHandler();
    VkQueue getGraphicsQueue() const;
    VkQueue getPresentQueue() const;
    VkSurfaceKHR& getSurface() const;
    PhysicalDevice getPhysicalDevice() const;
private:
    const PhysicalDevice &gpu;
    VkSurfaceKHR& surface;
    std::vector<const char *> enabledExtensions{};
    VkDevice handler{VK_NULL_HANDLE};
    VkQueue graphicsQueue;
    VkQueue presentQueue;
};

} // ScarlattiCore
