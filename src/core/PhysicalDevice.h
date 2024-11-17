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
    Instance &getInstance() const;
    VkPhysicalDevice getHandler();
    // TODO: implement a single extension verification
    bool isExtensionSupported(const std::string &extension) const;
    bool areExtensionsSupported(std::vector<char const *> required_extensions) const;
    VkPhysicalDeviceProperties getProperties() const;
    std::vector<VkQueueFamilyProperties> getQueueFamilyProperties() const;
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