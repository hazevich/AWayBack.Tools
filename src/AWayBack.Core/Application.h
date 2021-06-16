#pragma once

namespace AWayBack
{
	class Application
	{
	public:
		void Run();
	private:
		void OnClose();
	private:
		bool _isRunning = true;
	};
}