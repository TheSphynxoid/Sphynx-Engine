#pragma once
#include "Component.h"
#include "Core/Font.h"
#include <thread>
#include <future>
#include "FontAtlas.h"

namespace Sphynx::Core {
	class TextRenderer : public Component
	{
	private:
		Font* font;
		FontAtlas Atlas;
		std::thread TRenderThread;
		std::string String = "";
#ifdef DEBUG
		bool BasicMode;
#endif // DEBUG
		virtual void OnComponentAttach(GameObject* parent) override;
		void RenderLoop();
	public:
		TextRenderer(Font* font, std::string st);
		~TextRenderer();
		virtual void Update();
		void SetFont(Font* nfont) {
			font = nfont;
		}
		Font* GetFont() {
			return font;
		};
		void SetString(std::string& str) {
			String = str;
		}
		std::string& GetString() {
			return String;
		}
#ifdef DEBUG
		void SetBasicRendering(bool& val) { BasicMode = val; }
#endif //DEBUG
	};
}

