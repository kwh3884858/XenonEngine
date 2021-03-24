#pragma once

#include "CrossPlatform/Interface/DrawerType.h"
#include "CrossPlatform/Interface/IDrawerConfig.h"
#include "CrossPlatform/Interface/IDrawerSurface.h"

namespace CrossPlatform {

    class IDrawer
    {
    public:
        virtual ~IDrawer() {}

        virtual bool Initialize() = 0;
        virtual bool Shutdown() = 0;

        virtual DrawerType GetType() const = 0;
        virtual void SetDrawerConfig(IDrawerConfig* const config) = 0;
        virtual bool Draw(IDrawerSurface* const drawerSurface) = 0;
    };
}
