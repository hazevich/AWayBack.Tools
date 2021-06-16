#pragma once

#include <vector>
#include <functional>

namespace AWayBack
{
	template<typename T>
	class Event
	{
	public:
		Event& operator+=(std::function<void(const T&)> subscriber)
		{
			_subscribers.push_back(subscriber);

			return *this;
		}

		void Invoke(const T& event)
		{
			for (auto &subscriber : _subscribers)
			{
				subscriber(event);
			}
		}
	private:
		std::vector<std::function<void(const T&)>> _subscribers;
	};

	
	template<>
	class Event<void>
	{
	public:
		Event& operator+=(std::function<void()> subscriber)
		{
			_subscribers.push_back(subscriber);

			return *this;
		}

		void Invoke()
		{
			for (auto &subscriber : _subscribers)
			{
				subscriber();
			}
		}
	private:
		std::vector<std::function<void()>> _subscribers;
	};
}