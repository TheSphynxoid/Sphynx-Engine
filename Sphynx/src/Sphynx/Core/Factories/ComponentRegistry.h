#pragma once
#include <unordered_map>
#include <unordered_set>

namespace Sphynx {
	class GameObject;
	class Component;
}
namespace Sphynx::Core::Internal {
	//TODO: This needs to be obsoleted
	class ComponentRegistry
	{
	private:
		struct CompInfo {
			std::type_index typeindex;
			Sphynx::Component* comp_ptr;

			bool operator==(const CompInfo& rhs) const;
			bool operator!=(const CompInfo& rhs) const{
				return !((*this) == rhs);
			}
		};
		inline static std::unordered_map<size_t, std::unordered_set<CompInfo>> Registry;
	public:
		static void RegisterComp(Sphynx::GameObject* go, Sphynx::Component* comp);
		static void RemoveComp(Sphynx::GameObject* go, Sphynx::Component* comp);
		static bool IsCompInGO(Sphynx::GameObject* go, std::type_index typeindex);
		static bool IsCompInGO(Sphynx::GameObject* go, Sphynx::Component* comp);
		static void CopyGameObject(Sphynx::GameObject* ori, Sphynx::GameObject* newGO);
		static void MoveCompsToGameObject(const Sphynx::GameObject* Src, Sphynx::GameObject* Dest);
		friend std::hash<CompInfo>;
	};
}
template<>
struct std::hash<Sphynx::Core::Internal::ComponentRegistry::CompInfo> {
	size_t operator()(const Sphynx::Core::Internal::ComponentRegistry::CompInfo& k) const noexcept {
		return k.typeindex.hash_code();
	}
};