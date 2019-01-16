#pragma once

#include "PointerVector.hpp"

class TimerHandler
{
public:
	virtual void Invoke() = NULL;
};

class TimerManager
{
	vyt::vector<TimerHandler> m_handlers;
	static TimerManager m_instance;
	TimerManager() {}
public:
	template <typename TItem>
	size_t RegisterHandler()
	{
		auto index = m_handlers.size();
		m_handlers.Append<TItem>();
		return index;
	}
	void UnregiserHandler(size_t index)
	{
		m_handlers.Delete(index);
	}
};