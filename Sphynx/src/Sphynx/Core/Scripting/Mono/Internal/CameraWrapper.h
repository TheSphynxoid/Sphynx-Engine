#pragma once
#include "Camera.h"
#include "Scene.h"
#include "Core/SceneManager.h"
#include "Events/SceneEvents.h"


namespace Sphynx::Mono {
	class CameraWrapper final
	{
	public:
		static void Setup();
		static void SetPrimaryCamera(Events::OnSceneStart& e)noexcept;

		friend class MonoRuntime;
	};
}

