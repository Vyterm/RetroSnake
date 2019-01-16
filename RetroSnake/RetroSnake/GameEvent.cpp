//#include "GameEvent.hpp"
//
//EventManager EventManager::getInstance()
//{
//	return m_instance;
//}
//
//class __OverMessage : public __EventMessage
//{
//	int m_i;
//public:
//	__OverMessage(int i) : m_i(i) { }
//	int get_i() const { return m_i; }
//};
//using OverMessage = std::shared_ptr<__OverMessage>;
//
//
//OverMessage pom = std::make_shared<__OverMessage>(5);
//EventMessage em = pom;
//EventMessage em1 = std::make_shared<__EventMessage>();
//OverMessage om = std::dynamic_pointer_cast<__OverMessage>(em);
//EventMessage em2 = om;
////__EventMessage &em = __OverMessage(2);
//cout << om->get_i() << endl;
//cout << em->IsType<__OverMessage>() << endl;
//cin.get();