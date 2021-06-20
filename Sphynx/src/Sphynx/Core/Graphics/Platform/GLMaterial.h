#pragma once
#include "Core/Graphics/PipeLine/Material.h"

namespace Sphynx::Core::Graphics::GL{

    class GLRenderer;

    class GLMaterial : public Material
    {
    private:
        unsigned int ProgramId;
    public:
        GLMaterial(ShaderPack shaders,Texture* texture);
        virtual void Bind() override;

        friend GLRenderer;
    };
}