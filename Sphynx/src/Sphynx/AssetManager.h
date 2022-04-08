#pragma once
#include "Asset.h"
#include <fstream>
#include <unordered_map>

#ifndef Def_Asset_Folder
#define Def_Asset_Folder "data"
#endif

#define Path const char*

namespace Sphynx {
	//In-file Node.
	struct AssetNode {
		Path SectionPath = nullptr;
	};
	template<class T>
	class Serializer {

	};
	class AssetManager
	{
	private:
		struct AssetInfo {

		};
		inline static std::unordered_map<Path, Path> FileMap;
		inline static std::unordered_map<size_t, AssetInfo*> RegisteredAssetsTypes;
	public:
		static void InitMapping() {
		}
		static void SetPathingMap(std::unordered_map<Path, Path> map) {
			FileMap.insert(map.begin(), map.end());
			FileMap.swap(map);
		}
		static Asset* GetAsset(Path path) {
			std::fstream file = std::fstream(path, std::ios::binary | std::ios::in);
			char magic[4];
			file.read(magic, sizeof(magic));
			if (strncmp(magic, "SALF", 4) == 0) {
				size_t hash;
				file.read((char*)&hash, sizeof(size_t));
				auto info = RegisteredAssetsTypes[hash];
			}
		}
		//loose file
		static void SaveAsset(Asset asset, Path path) {
			std::fstream file = std::fstream(path, std::ios::binary | std::ios::out);
			file << "SALF";
			file.close();
		}
		//Regsiters the class For Asset Serialization and Derialization. Uses Custom Hash if Needed.
		template<class T>
		static void RegisterAssetType(const size_t Hash = 0) {
			if (!Hash) {
				size_t hash = typeid(T).hash_code();
			}
		}

	};
}