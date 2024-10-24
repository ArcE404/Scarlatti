//
// Created by user on 22/10/2024.
//

#ifndef DEVICE_H
#define DEVICE_H
#include "../Common.h"
#include "SwapChain.h"

namespace Scarlatti {
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    class Device {
    private:
        // the index of the queue families
        QueueFamilyIndices queueFamilies;

        // the physical device, in this case a GPU with the necessary hardware
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;

        // This is the logial device, it is an interface of the physical one that we choose before
        VkDevice logicalDevice;

        // this is the graphic queue, here will be queue all the graphics commands because Vulkan is async
        VkQueue graphicsQueue;

        // this is the queue that will handle the presentation commands (could be the same as the graphic one but for a more
        // uniform aproach we prefer to separate them
        VkQueue presentQueue;

        VkInstance instance = VK_NULL_HANDLE;

        // methods
        void pickPhysicalDevice();
        void createLogicalDevice();
        bool isDeviceSuitable(VkPhysicalDevice);
        bool checkDeviceExtensionSupport(VkPhysicalDevice);
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice);
    public:
        Device(VkInstance);
        ~Device();
        VkPhysicalDevice getPhysicalDevice();
        VkDevice getLogicalDevice();
        QueueFamilyIndices getQueueFamilies();
    };

} // Scarlatti

#endif //DEVICE_H
