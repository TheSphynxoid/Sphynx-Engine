#pragma once
#include <unordered_map>
#include <unordered_set>

namespace Sphynx {
	class GameObject;
	class Component;
}
namespace Sphynx::Core::Internal {
	typedef Component* (*CreateComponentFunc)();

	class ComponentRegistry
	{
	private:
		struct CompInfo {
			std::type_index typeIndex;
			Sphynx::Component* comp_ptr;

			bool operator==(const CompInfo& rhs) const noexcept;
			bool operator!=(const CompInfo& rhs) const noexcept {
				return !((*this) == rhs);
			}
		};
		//Maps GameObject pointer to Component information.
		inline static std::unordered_map<size_t, std::unordered_set<CompInfo>> InstanceRegistry;
		//Maps Component names to factory methods.
		inline static std::unordered_map<std::string, CreateComponentFunc> TypeRegistry;
	public:
		static void RegisterComp(Sphynx::GameObject* go, Sphynx::Component* comp)noexcept;
		static void RemoveComp(Sphynx::GameObject* go, Sphynx::Component* comp)noexcept;
		static bool IsCompInGO(Sphynx::GameObject* go, std::type_index typeindex)noexcept;
		static bool IsCompInGO(Sphynx::GameObject* go, Sphynx::Component* comp)noexcept;
		static void CopyGameObject(Sphynx::GameObject* ori, Sphynx::GameObject* newGO)noexcept;
		static void MoveCompsToGameObject(const Sphynx::GameObject* Src, Sphynx::GameObject* Dest)noexcept;
		friend std::hash<CompInfo>;
		template<typename T>
		friend struct RegistryEntry;
	};
}
template<>
struct std::hash<Sphynx::Core::Internal::ComponentRegistry::CompInfo> {
	size_t operator()(const Sphynx::Core::Internal::ComponentRegistry::CompInfo& k) const noexcept {
		return k.typeIndex.hash_code();
	}
};