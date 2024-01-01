#pragma once
#ifndef SPH_ID
#define SPH_ID
#include <limits>
#include <queue>
#include <list>

typedef unsigned short int ushort;
typedef unsigned char uchar;
namespace Sphynx {
	class Object;
	enum class EntityFlag : uchar {
		Void = 0x0,
		GameObject = 0x1,
		Component =  0x2,
		Reserved1 = 0x4,
		UserDefined = 0x8
	};
	struct EntityID {
		ushort InstanceID;
		ushort GenID;
		ushort Userflag;
		uchar Pad;
		uchar Flag;
		EntityID() = default;
		explicit EntityID(size_t value) noexcept {
			InstanceID = static_cast<unsigned short>((value >> 0) & 0xFFFF);
			GenID = static_cast<unsigned short>((value >> 16) & 0xFFFF);
			Userflag = static_cast<unsigned short>((value >> 32) & 0xFFFF);
			Pad = static_cast<unsigned char>((value >> 48) & 0xFF);
			Flag = static_cast<unsigned char>((value >> 56) & 0xFF);
		}
		operator size_t() const noexcept {
			long long result = 0;

			result |= static_cast<long long>(InstanceID) << 0;
			result |= static_cast<long long>(GenID) << 16;
			result |= static_cast<long long>(Userflag) << 32;
			result |= static_cast<long long>(Pad) << 48;
			result |= static_cast<long long>(Flag) << 56;

			return result;
		}

		bool operator ==(EntityID& rref) const {
			return (int)*this == (int)rref;
		}
	};
	//Utility.
	union ID {
		EntityID EID;
		size_t NumericID = 0;
	};
	inline bool operator==(const ID& left, const ID& right) noexcept {
		return left.NumericID == right.NumericID;
	}
	//For now, Object and generation Limit is 65536 which is sufficent enough (65536^2 Unique Entities).
	class EntityRegistry {
	private:
		inline static std::queue<ID> FreeIDs;
		inline static std::list<ID> IDs;
		inline static ushort LastInstanceID = 0;
		inline static std::map<EntityID, Object*> ObjectMap;
	public:
		static EntityID& GetID(Object* Obj,EntityFlag flag) {
			if (FreeIDs.empty()) {
				ID id;
				//This Will happen this first generations only.
				//thus, we are setting the value as the InstanceID + 1  
				id.EID.InstanceID = (LastInstanceID++);
				id.EID.Flag = (uchar)flag;
				IDs.push_back(id);
				ObjectMap[id.EID] = Obj;
				return id.EID;
			}
			else {
				auto& id = FreeIDs.front();
				FreeIDs.pop();
				id.EID.GenID += 1;
				id.EID.Flag = (uchar)flag;
				IDs.push_back(id);
				return id.EID;
			}
		}
		static void ReturnID(EntityID& eid) {
			ID id;
			id.EID = eid;
			FreeIDs.push(id);
			IDs.remove(id);
			ObjectMap[eid] = nullptr;
		}
		static Object* GetObjectByID(EntityID& eid) {
			return ObjectMap[eid];
		}

	};
}
#endif