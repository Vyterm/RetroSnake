#pragma once

#include "PointerVector.hpp"

#include <functional>

#include <ctime>


class TimerManager
{
public:
	class handler
	{
	private:
		clock_t m_timer;
		clock_t m_invokeTime;
		bool m_isLoop;
	protected:
		handler(clock_t invokeTime, bool isLoop) : m_invokeTime(invokeTime), m_isLoop(isLoop), m_timer(m_invokeTime) { }
	public:
		virtual ~handler() { }
		virtual void Invoke() = NULL;
		bool Clock(clock_t timeDelta)
		{
			m_timer -= timeDelta;
			if (m_timer > 0) return false;
			Invoke();
			if (!m_isLoop) return true;
			m_timer += m_invokeTime;
			return false;
		}
		void Reset(clock_t invokeTime)
		{
			Reset(invokeTime, m_isLoop);
		}
		void Reset(clock_t invokeTime, bool isLoop)
		{
			m_invokeTime = invokeTime;
			m_timer = invokeTime;
			m_isLoop = isLoop;
		}
	};
	static TimerManager& get_instance() { return m_instance; }
	template <typename TItem>
	handler& RegisterHandler(clock_t invokeTime, bool isLoop)
	{
		auto index = m_handlers.size();
		m_handlers.Append<TItem>(invokeTime, isLoop);
		return m_handlers[index];
	}
	template <typename TItem, typename TArg1>
	handler& RegisterHandler(clock_t invokeTime, bool isLoop, TArg1 &arg1)
	{
		auto index = m_handlers.size();
		m_handlers.Append<TItem>(invokeTime, isLoop, arg1);
		return m_handlers[index];
	}
	void UnregiserHandler(handler &handler)
	{
		m_handlers.DeleteByReference(&handler);
	}
	void HandleClock()
	{
		auto newTimer = clock();
		auto timedelta = newTimer - m_timer;
		m_timer = newTimer;
		std::vector<handler*> deleteVector;
		for (auto &handler : m_handlers)
		{
			if (handler.Clock(timedelta))
				deleteVector.push_back(&handler);
		}
		for (auto &handler : deleteVector)
			m_handlers.DeleteByReference(handler);
	}
private:
	clock_t m_timer;
	vyt::vector<handler> m_handlers;
	static TimerManager m_instance;
	TimerManager() {}
};