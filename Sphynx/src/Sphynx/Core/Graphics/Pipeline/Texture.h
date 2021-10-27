#pragma once

namespace Sphynx::Core::Graphics {
	enum class TextureWrappingMode {
		Repeat, MirroredRepeat, ClampToEdge, ClampToBorder
	};
	enum class TextureFilterMode {
		Nearest,Linear
	};
	enum class TextureMipmapMode {
		NearestMipmapNearest, NearestMipmapLinear, LinearMipmapNearest, LinearMipmapLinear
	};
	enum class TextureFormat {
		DepthComponent, DepthStencil, Red, RG, RGB, RGBA, CompressedRed, CompressedRG, CompressRGB, CompressedRGBA
	};
	enum class TextureType {
		Texture2D, Texture3D, CubeMap, Rectangle
	};
	class Texture
	{
		//Mipmap level 0
		Texture* Create(const char* path);
		Texture* Create(const char* path, TextureType Type, int MipmapLevel, TextureFormat format, TextureWrappingMode warp, 
			TextureFilterMode filter, TextureMipmapMode Mipmap);
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
<<<<<<< Updated upstream
=======
		void MarkForCleanup() { DeleteFlag = true; };

		friend class IRenderer;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	};
}