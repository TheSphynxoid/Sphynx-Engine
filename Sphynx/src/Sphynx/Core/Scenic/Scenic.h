#pragma once
#include <fstream>
#include "Scene.h"
#include "Core/IO/File.h"
#include "Core/Factories/ComponentFactory.h"
#include "Core/MeshRenderer.h"
#include "Camera.h"
#include "Core/Exceptions.h"

#define ScenicVersion "0.01v"

namespace Sphynx {
	class Scenic
	{
	private:
		struct FileHeader {
			const char Magic[4] = { 'S','P','H','S' };
			const int SceanicVer = 0001;
			const int SceanicParserVer = 0001;
		}static header;
	public:
		static Core::Scene ReadScene(const char* path) {

		};
		static void WriteScene(Core::Scene* scene, std::fstream file) {

		};
	};
}