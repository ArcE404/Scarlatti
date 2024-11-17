//
// Created by user on 09/11/2024.
//
#pragma once
namespace ScarlattiCore {
    class Device;
}

namespace ScarlattiResources {
    class DeviceResourceCache {
        public:
        DeviceResourceCache(ScarlattiCore::Device& _device);
        private:
        ScarlattiCore::Device& device;
};
} // ScarlattiResources
