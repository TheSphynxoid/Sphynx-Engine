#pragma once
#include "Core/Graphics/Imgui.h"
namespace Sphynx::Editor {
    class SceneManagerView : public Sphynx::Core::IOverlayWindow
    {
    public:
        SceneManagerView();
        // Inherited via IOverlayWindow
        virtual void Draw() override;
    };
}

