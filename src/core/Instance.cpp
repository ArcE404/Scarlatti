//
// Created by proxy on 25/10/2024.
//

#include "Instance.h"

//TODO: verify why the VK_LAYER_KHRONOS_validation are logged twice

namespace ScarlattiCore {
    bool enableExtension(const char *requiredExtensionName,
                         const std::vector<VkExtensionProperties> &availableExtensions,
                         std::vector<const char *> *enabledExtensions) {
        for (auto &availableExtension: availableExtensions) {
            if (strcmp(availableExtension.extensionName, requiredExtensionName) == 0) {
                auto it = std::find_if(enabledExtensions->begin(), enabledExtensions->end(),
                                       [requiredExtensionName](const char *enabledExtensionName) {
                                           return strcmp(enabledExtensionName, requiredExtensionName) == 0;
                                       });

                if (it != enabledExtensions->end()) {
                    // Extension is already enabled
                } else {
                    const std::string message = std::string("Extension ") + requiredExtensionName + " found, enabling it";
                    Common::LOG(message);
                    enabledExtensions->emplace_back(requiredExtensionName);
                }
                return true;
            }
        }

        const std::string message = std::string("Extension ") + requiredExtensionName + " not found";
        Common::LOG(message, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
        return false;
    }

    bool validateLayers(const std::vector<const char *> &required,
                        const std::vector<VkLayerProperties> &available) {
        for (auto layer: required) {
            bool found = false;
            for (auto &available_layer: available) {
                if (strcmp(available_layer.layerName, layer) == 0) {
                    found = true;
                    break;
                }
            }

            if (!found) {
                const std::string message = std::string("Validation layer ") + layer + " not found";
                Common::LOG(message, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
                return false;
            }
        }

        return true;
    }

    std::vector<const char *> getOptimalValidationLayers(
        const std::vector<VkLayerProperties> &supported_instance_layers) {
        std::vector<std::vector<const char *> > validation_layer_priority_list =
        {
            // The preferred validation layer is "VK_LAYER_KHRONOS_validation"
            {"VK_LAYER_KHRONOS_validation"},

            // Otherwise we fallback to using the LunarG meta layer
            {"VK_LAYER_LUNARG_standard_validation"},

            // Otherwise we attempt to enable the individual layers that compose the LunarG meta layer since it doesn't exist
            {
                "VK_LAYER_GOOGLE_threading",
                "VK_LAYER_LUNARG_parameter_validation",
                "VK_LAYER_LUNARG_object_tracker",
                "VK_LAYER_LUNARG_core_validation",
                "VK_LAYER_GOOGLE_unique_objects",
            },

            // Otherwise as a last resort we fallback to attempting to enable the LunarG core layer
            {"VK_LAYER_LUNARG_core_validation"}
        };

        for (auto &validation_layers: validation_layer_priority_list) {
            if (validateLayers(validation_layers, supported_instance_layers)) {
                return validation_layers;
            }

            Common::LOG("Couldn't enable validation layers (see log for error) - falling back");
        }

        // Else return nothing
        return {};
    }

    void setUpValidationLayers(std::vector<const char *> &requested_validation_layer) {
        // We extract the layers that our instance can handle
        uint32_t instanceLayerCount;
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);

        std::vector<VkLayerProperties> supportedInstanceLayers(instanceLayerCount);
        vkEnumerateInstanceLayerProperties(&instanceLayerCount, supportedInstanceLayers.data());

        // We ask for the optimal validation layers, we must ensure that the optimal are supported by the instance so
        // we send the supportedInstanceLayer to the method. Then if they exist we add them to the requested validation layers
        std::vector<const char *> optimal_validation_layers = getOptimalValidationLayers(supportedInstanceLayers);
        requested_validation_layer.insert(requested_validation_layer.end(), optimal_validation_layers.begin(),
                                         optimal_validation_layers.end());

        // We need to make a secondary validation to ensure that the requested layers by the user are also supported
        if (validateLayers(requested_validation_layer, supportedInstanceLayers)) {
            Common::LOG("Enabled Validation Layers:");
            for (const auto &layer: requested_validation_layer) {
                const std::string message = std::string("	\t") + layer;
                Common::LOG(message);
            }
        } else {
            throw std::runtime_error("Required validation layers are missing.");
        }
    }

    Instance::Instance(const std::string &application_name,
                       const std::unordered_map<const char *, bool> &required_extensions,
                       const std::vector<const char *> &required_validation_layers,
                       uint32_t api_version) {
        // We extract first the extensions that our instance can handle
        uint32_t instanceExtensionCount;
        vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount, nullptr);

        std::vector<VkExtensionProperties> availableInstanceExtensions(instanceExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &instanceExtensionCount,
                                               availableInstanceExtensions.data());

        // we create the application info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = application_name.c_str();
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = api_version;

        // we set the application info to global
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // we make an iteration to the required extensions, we use the availableInstanceExtensions list to verify
        // that we have that extension available in our instance
        auto extension_error = false;
        for (auto extension: required_extensions) {
            auto extensionName = extension.first;
            auto extensionIsOptional = extension.second;
            // if the extension exist in the available extensions, we pass the class variable enabledExtensions to save it
            if (!enableExtension(extensionName, availableInstanceExtensions, &enabledExtensions)) {
                if (extensionIsOptional) {
                    const std::string message = std::string("Optional instance extension ") + extensionName +
                                                " not available, some features may be disabled";
                    Common::LOG(message, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
                } else {
                    const std::string message = std::string("Required instance extension ") + extensionName +
                                                " not available, cannot run";
                    Common::LOG(message, VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT);
                    extension_error = true;
                }
                extension_error = extension_error || !extensionIsOptional;
            }
        }

        // global extensions, since vulkan is platform-agnostic (we need to specify the extensions so vulkan
        // is able to communicate with the window)
        createInfo.enabledExtensionCount = static_cast<uint32_t>(enabledExtensions.size());
        createInfo.ppEnabledExtensionNames = enabledExtensions.data();

        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        std::vector<const char *> requestedValidationLayers(required_validation_layers);
        if(Common::isDebugEnabled()) {
            Common::LOG("Creating debug messenger");
            // We map the required validation layer (the parameter) to a variable, as requested validation layers
            setUpValidationLayers(requestedValidationLayers);

            // if active, add the validation layer to the vk instance.
            createInfo.enabledLayerCount = static_cast<uint32_t>(requestedValidationLayers.size());
            createInfo.ppEnabledLayerNames = requestedValidationLayers.data();

            Common::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *) &debugCreateInfo;
        }else {
            createInfo.enabledLayerCount = 0;
            createInfo.pNext = nullptr;
        }

        if (vkCreateInstance(&createInfo, nullptr, &handler) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }

    Instance::~Instance() {
    }

    VkInstance Instance::getHandler() {
        return handler;
    }
} // ScarlattiCore
