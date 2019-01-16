//#pragma once
//#include <memory>
//#include <vector>
//
//class __EventMessage : public std::enable_shared_from_this<__EventMessage>
//{
//public:
//	template <class T>
//	bool IsType() 
//	{
//		auto thisType = typeid(this).name;
//		auto tType = typeid(T).name;
//		return thisType == tType;
//	}
//	virtual ~__EventMessage() { }
//};
//
//class __EventHandler : public std::enable_shared_from_this<__EventHandler>
//{
//public:
//	void Dispatch();
//	virtual ~__EventHandler() { }
//};
//
//class EventManager
//{
//	EventManager() {}
//	static EventManager m_instance;
//public:
//	static EventManager getInstance();
//};
//
//using EventHandler = std::shared_ptr<__EventHandler>;
//using EventMessage = std::shared_ptr<__EventMessage>;