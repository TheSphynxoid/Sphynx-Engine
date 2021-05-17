#pragma once
#include "Core/Graphics/Window.h"
#include "Vector.h"
namespace Sphynx::Core::Graphics {
	//Base Class Of Renderers.
	class IRenderer
	{
		// Inherited via Module
		virtual void Start(IWindow* app) = 0;
		virtual void Render();
		virtual void Clear();
		//Send Data,Shaders,Indexes,Textures (Make A Object that encapsulates them all).
		virtual void Submit(/*RenderObject robj*/);
	};
}