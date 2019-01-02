#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H
#include "function_traits.h"
#include <map>
#include <any>
#include <type_traits>
class MessageBus
{
public:
    MessageBus();
    MessageBus(const MessageBus&) = delete;

    template<class F>
    void attach(int type, F&& f)
    {
        auto func = to_function(std::forward<F>(f));
        add(type, func);
    }

    void remove(int type)
    {
        auto range = m_map.equal_range(type);
        m_map.erase(range.first, range.second);
    }

    template<class R>
    void sendMsg(int type)
    {
        using stl_function = std::function<R()>;
        auto range = m_map.equal_range(type);
        for (auto itr = range.first; itr != range.second; itr++) {
            std::any_cast<stl_function>(itr->second)();
        }
    }

    template<class R, class... Args>
    void sendMsg(int type, Args... args)
    {
        using stl_function = std::function<R(Args...)>;
        auto range = m_map.equal_range(type);
        for (auto itr = range.first; itr != range.second; itr++) {
            std::any_cast<stl_function>(itr->second)(std::forward<Args>(args)...);
        }
    }
private:
    std::multimap<int, std::any> m_map;
    using iterator = decltype(m_map)::iterator;
private:
    template<class F>
    void add(int type, F&& f)
    {
        m_map.emplace(std::move(type), std::any(std::forward<F>(f)));
    }
};

#endif // MESSAGEBUS_H
