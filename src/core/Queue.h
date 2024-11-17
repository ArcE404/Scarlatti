//
// Created by user on 05/11/2024.
//
#pragma once
#include "Device.h"
namespace ScarlattiCore {

class Queue {
public:
    Queue(Device& device, uint32_t family_index, VkQueueFamilyProperties properties, VkBool32 can_present, uint32_t index);
    VkQueue getHandler() const;

private:
    Device &device;
    VkQueue handler{VK_NULL_HANDLE};
    uint32_t family_index{0};
    uint32_t index{0};
    VkBool32 can_present{VK_FALSE};
    VkQueueFamilyProperties properties{};
};

} // ScarlattiCore

