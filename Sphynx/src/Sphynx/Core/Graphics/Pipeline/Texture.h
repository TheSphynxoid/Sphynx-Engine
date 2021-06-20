#pragma once

namespace Sphynx::Core::Graphics {
	class Texture
	{
		virtual void Create() = 0;
		virtual void Bind() = 0;
	};
}