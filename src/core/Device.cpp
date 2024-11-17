//
// Created by user on 05/11/2024.
//

#include "Device.h"

namespace ScarlattiCore {
    Device::Device(PhysicalDevice &gpu, VkSurfaceKHR surface,
        std::vector<const char *> requested_extensions): gpu{gpu}, resourceCache{*this}{
        QueueFamilyIndices indices = gpu.getInstance().getQueueFamilies();

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        createInfo.pEnabledFeatures = &deviceFeatures;

        createInfo.enabledExtensionCount = static_cast<uint32_t>(requested_extensions.size());
        createInfo.ppEnabledExtensionNames = requested_extensions.data();

        if (Common::isDebugEnabled()) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(gpu.getInstance().getValidationLayers().size());
            createInfo.ppEnabledLayerNames = gpu.getInstance().getValidationLayers().data();
        } else {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateDevice(gpu.getHandler(), &createInfo, nullptr, &handler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        vkGetDeviceQueue(handler, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(handler, indices.presentFamily.value(), 0, &presentQueue);
    }

    // find on the fly a queue family index that is not stored in the instance's family index struct
    uint32_t Device::getQueueFamilyIndex(VkQueueFlagBits queue_flag_bits) {
        const auto &queueFamilyProperties = gpu.getQueueFamilyProperties();
        // Dedicated queue for compute
        // Try to find a queue family index that supports compute but not graphics
        if (queue_flag_bits & VK_QUEUE_COMPUTE_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
            {
                if ((queueFamilyProperties[i].queueFlags & queue_flag_bits) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    return i;
                    break;
                }
            }
        }

        // Dedicated queue for transfer
        // Try to find a queue family index that supports transfer but not graphics and compute
        if (queue_flag_bits & VK_QUEUE_TRANSFER_BIT)
        {
            for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
            {
                if ((queueFamilyProperties[i].queueFlags & queue_flag_bits) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) && !(queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT))
                {
                    return i;
                    break;
                }
            }
        }

        // For other queue types or if no separate compute queue is present, return the first one to support the requested flags
        for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++)
        {
            if (queueFamilyProperties[i].queueFlags & queue_flag_bits)
            {
                return i;
                break;
            }
        }

        throw std::runtime_error("Could not find a matching queue family index");
    }

    VkDevice Device::getHandler() {
        return handler;
    }

    VkQueue Device::getGraphicsQueue() const {
        return graphicsQueue;
    }

    VkQueue Device::getPresentQueue() const {
        return presentQueue;
    }
} // ScarlattiCore