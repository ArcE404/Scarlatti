//
// Created by user on 25/10/2024.
//

#pragma once

#include "../Common.h"
namespace ScarlattiCore {
class PhysicalDevice;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class Instance {
public:
    Instance(const std::string                            &application_name,
                 const std::unordered_map<const char *, bool> &required_extensions        = {},
                 const std::vector<const char *>              &required_validation_layers = {},
                 uint32_t                                      api_version                = VK_API_VERSION_1_0);
    ~Instance();
    VkInstance getHandler();
    void queryGpus();

    PhysicalDevice &getSuitableGpu(VkSurfaceKHR _surface, std::vector<char const *> required_extensions);
    PhysicalDevice &getFirstGpu();
    QueueFamilyIndices &getQueueFamilies();
    SwapChainSupportDetails &getSwapChainSupport();
private:
    bool isDeviceSuitable(PhysicalDevice physical_device, VkSurfaceKHR _surface, std::vector<char const *> required_extensions);
    void findQueueFamiliesIndices(PhysicalDevice physical_device, VkSurfaceKHR _surface);
    void querySwapChainSupport(PhysicalDevice _device, VkSurfaceKHR _surface);
    VkInstance  handler{VK_NULL_HANDLE};
    QueueFamilyIndices queueFamiliesIndices;
    SwapChainSupportDetails swapChainSupportDetails;
    std::vector<const char *> enabledExtensions;
    std::pmr::vector<std::unique_ptr<PhysicalDevice>> gpus;
    VkDebugUtilsMessengerEXT debugMessenger{VK_NULL_HANDLE};
};
} // ScarlattiCore
