#pragma once

#include "ImGuiPlatformBackend.h"

namespace AWayBack
{
	class ImGuiRenderer
	{
	public:
		ImGuiRenderer(void* window);
		~ImGuiRenderer();

		void Initialize();
		void NewFrame();
		void Render();

	private:
		ImGuiPlatformBackend* _imGuiPlatformBackend;
	};
}