//
// Created by user on 05/11/2024.
//

#include "Queue.h"

namespace ScarlattiCore {
    Queue::Queue(Device &device, uint32_t family_index, VkQueueFamilyProperties properties, VkBool32 can_present,
        uint32_t index)
    :
        device{device},
        family_index{family_index},
        index{index},
        can_present{can_present},
        properties{properties}
    {
        vkGetDeviceQueue(device.getHandler(), family_index, index, &handler);
    }

    VkQueue Queue::getHandler() const {
        return handler;
    }
} // ScarlattiCore