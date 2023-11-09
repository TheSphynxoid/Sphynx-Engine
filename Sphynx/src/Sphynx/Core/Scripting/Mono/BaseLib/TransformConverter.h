#pragma once
#include "../Marshaller.h"
#include "Core/Transform.h"
#include "GameObject.h"

namespace Sphynx::Mono {

	class TransformConverter
	{
	private:
		Transform* transform;

	public:
		TransformConverter(Transform* tr) : transform(tr) {
			
		}
	};

}

