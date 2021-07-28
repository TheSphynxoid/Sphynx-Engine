#pragma once

namespace Sphynx::Core::Graphics {
	class Texture
	{	
		virtual void CreateTexture() = 0;
		virtual void Bind() = 0;
	};
}