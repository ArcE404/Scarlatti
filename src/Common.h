//
// Created by user on 23/10/2024.
//

#ifndef COMMON_H
#define COMMON_H

#endif //COMMON_H

// these are all the necessary includes

#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <cstring>
#include <optional>
#include <set>
#include <cstdint> // Necessary for uint32_t
#include <limits> // Necessary for std::numeric_limits
#include <algorithm> // Necessary for std::clamp
#include <fstream>
#include <filesystem>
#include <unordered_map>
using namespace std;
class Common {
public:
    static void LOG(std::string message, VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
        switch (messageSeverity) {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            {
                std::cout << "LOG: " << message << std::endl;
                break;
            }
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            {
                std::cerr << "LOG: " << message << std::endl;
                break;
            }
            default: std::clog << "Not valid severity message bit" << std::endl;
        }
    }

    bool static isDebugEnabled() {
#ifdef NDEBUG
        const bool enableValidationLayers = false;
#else
        constexpr bool enableValidationLayers = true;
#endif
        return enableValidationLayers;
    }
};
