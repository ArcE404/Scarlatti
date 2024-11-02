//
// Created by user on 25/10/2024.
//
#pragma once

#include "Instance.h"
namespace ScarlattiCore {
    class Instance;
class PhysicalDevice {
public:
    PhysicalDevice(Instance &_instance, VkPhysicalDevice physical_device);
    ~PhysicalDevice();
    Instance &GetInstance() const;
    VkPhysicalDevice getHandler();
private:
    Instance& instance;
    VkPhysicalDevice handler{VK_NULL_HANDLE};
    VkPhysicalDeviceFeatures features{};
    VkPhysicalDeviceProperties properties;
    VkPhysicalDeviceMemoryProperties memoryProperties;
    std::vector<VkQueueFamilyProperties> queueFamilyProperties;
    std::vector<VkExtensionProperties> deviceExtensions;
};

} // ScarlattiCore