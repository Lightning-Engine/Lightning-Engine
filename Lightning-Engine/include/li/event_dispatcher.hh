#ifndef EVENT_DISPATCHER_HH
#define EVENT_DISPATCHER_HH

#include <vector>
#include <functional>

namespace li {
	template<typename Event>
	class dispatcher_base {
	public:
		std::vector<std::function<void(Event &)>> handlers;
	};

	template<typename Base, typename Derived, bool IsBaseOf = std::is_base_of<Base, Derived>::value>
	class dispatcher_pusher {
	public:
		static void push(dispatcher_base<Derived> &dispatcher, std::function<void(Base &)> handler) {
			(void) dispatcher;
			(void) handler;
		}
	};

	template<typename Base, typename Derived>
	class dispatcher_pusher<Base, Derived, true> {
	public:
		static void push(dispatcher_base<Derived> &dispatcher, std::function<void(Base &)> handler) {
			dispatcher.handlers.push_back(handler);
		}
	};

	template<typename... Events>
	class event_dispatcher : private dispatcher_base<Events>... {
	public:
		template<typename Event>
		void operator()(Event &event) {
			for (auto &handler : dispatcher_base<Event>::handlers) {
				handler(event);
			}
		}

		template<typename Event>
		void add_callback(std::function<void(Event &)> handler) {
			(void) (std::initializer_list<int> { (
				dispatcher_pusher<Event, Events>::push(*this, handler), 0
			)... });
		}

		template<typename Event, typename Listener>
		void add_listener(Listener &listener) {
			add_callback<Event>(std::bind(static_cast<void(Listener::*)(Event &)>(&Listener::on), &listener, std::placeholders::_1));
		}
	};
}

#endif