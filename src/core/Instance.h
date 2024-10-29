//
// Created by user on 25/10/2024.
//

#ifndef INSTANCE_H
#define INSTANCE_H

#include "PhysicalDevice.h"
#include "../Common.h"
namespace ScarlattiCore {

class Instance {
public:
    Instance(const std::string                            &application_name,
                 const std::unordered_map<const char *, bool> &required_extensions        = {},
                 const std::vector<const char *>              &required_validation_layers = {},
                 uint32_t                                      api_version                = VK_API_VERSION_1_0);
    ~Instance();
    VkInstance getHandler();
private:
    VkInstance handler{VK_NULL_HANDLE};

    std::vector<const char *> enabledExtensions;
    std::pmr::vector<std::unique_ptr<PhysicalDevice>> gpus;
};

} // ScarlattiCore

#endif //INSTANCE_H
