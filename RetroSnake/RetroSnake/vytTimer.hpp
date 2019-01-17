#ifndef VYT_TIMER_HPP
#define VYT_TIMER_HPP

#include "vytVector.hpp"

#include <functional>
#include <ctime>

namespace vyt
{
	class timer
	{
	public:
		class handler
		{
		private:
			clock_t m_timer;
			clock_t m_invokeTime;
			bool m_isLoop;
		protected:
			handler(clock_t invokeTime, bool isLoop) : m_invokeTime(invokeTime), m_isLoop(isLoop), m_timer(invokeTime) { }
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
			void StopTimer()
			{
				Reset(0, false);
			}
		};
		template <typename TItem>
		handler& RegisterHandler()
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>();
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1>
		handler& RegisterHandler(TArg1 &arg1)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1);
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1, typename TArg2>
		handler& RegisterHandler(TArg1 &arg1, const TArg2 &arg2)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1, arg2);
			return m_handlers[index];
		}
		template <typename TItem, typename TArg1, typename TArg2, typename TArg3>
		handler& RegisterHandler(TArg1 &arg1, const TArg2 &arg2, const TArg3 &arg3)
		{
			auto index = m_handlers.size();
			m_handlers.Append<TItem>(arg1, arg2, arg3);
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
		static timer& get_instance()
		{
			if (nullptr == m_instance)
				m_instance = new timer();
			return *m_instance;
		}
	private:
		clock_t m_timer;
		vyt::vector<handler> m_handlers;
		static timer *m_instance;
		timer() {}
	};
}

#endif