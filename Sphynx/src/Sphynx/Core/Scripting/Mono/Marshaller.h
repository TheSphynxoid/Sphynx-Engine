#pragma once
#include "mono/metadata/object.h"
#include "mono/metadata/class.h"

namespace Sphynx::Mono {
	class Mono {
	public:
		template<class T>
		T* CreateFromObj(MonoObject*) {

		}
	};
	class IMonoConverter {
	public:

	};
}