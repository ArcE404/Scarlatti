//
// Created by user on 25/10/2024.
//

#include "PhysicalDevice.h"

namespace ScarlattiCore {
    PhysicalDevice::PhysicalDevice(Instance &_instance, VkPhysicalDevice physical_device) :
    instance{_instance}, handler{physical_device} {
        // we get the information of the physical device
        vkGetPhysicalDeviceFeatures(physical_device, &features);
        vkGetPhysicalDeviceProperties(physical_device, &properties);
        vkGetPhysicalDeviceMemoryProperties(physical_device, &memoryProperties);
        Common::LOG(std::string("Found GPU: ") + properties.deviceName);

        uint32_t queue_family_properties_count = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_count, nullptr);
        queueFamilyProperties = std::vector<VkQueueFamilyProperties>(queue_family_properties_count);
        vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queue_family_properties_count, queueFamilyProperties.data());

        uint32_t device_extension_count;
        vkEnumerateDeviceExtensionProperties(getHandler(), nullptr, &device_extension_count, nullptr);
        deviceExtensions = std::vector<VkExtensionProperties>(device_extension_count);
        vkEnumerateDeviceExtensionProperties(getHandler(), nullptr, &device_extension_count, deviceExtensions.data());

        if (deviceExtensions.size() > 0)
        {
            Common::LOG("Device supports the following extensions:");
            for (auto &extension : deviceExtensions)
            {
                Common::LOG(std::string("  \t{}") + extension.extensionName);
            }
        }
    }

    Instance & PhysicalDevice::GetInstance() const {
        return instance;
    }

    VkPhysicalDevice PhysicalDevice::getHandler() {
        return handler;
    }

    bool PhysicalDevice::areExtensionsSupported(std::vector<const char *> required_extensions) const {
        std::set<std::string> requiredExtensions(required_extensions.begin(), required_extensions.end());

        for (const auto& extension : deviceExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }

        return requiredExtensions.empty();
    }

    VkPhysicalDeviceProperties PhysicalDevice::getProperties() const {
        return properties;
    }

    PhysicalDevice::~PhysicalDevice() {
    }
} // ScarlattiCore