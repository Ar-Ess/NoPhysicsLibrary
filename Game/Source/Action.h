#pragma once
#include "Array.h"
#include <functional>

template<class ...Ts>
class Action
{
public:

    Action() {}

    ~Action() { listeners.Clear(); }

    void Invoke(Ts... value)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
            listeners[i](value...);
    }

    // For global functions:
    //      operator+= std::bind(FuncName, args);
    // For member functions:
    //      operator+= std::bind(&Class::FuncName, PtrToClass, args);
    void operator+=(std::function<void(Ts...)> func)
    {
        listeners.Add(func);
    }

    // For global functions:
    //      operator-= std::bind(FuncName, args);
    // For member functions:
    //      operator-= std::bind(&Class::FuncName, PtrToClass, args);
    void operator-=(std::function<void(Ts...)> func)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
        {
            if (listeners[i].target_type() == func.target_type())
            {
                listeners.Erase(i);
                break;
            }
        }
    }

    bool Exists(std::function<void(Ts...)> func)
    {
        if (listeners.Empty()) return false;

        for (int i = 0; i < listeners.Size(); ++i)
        {
            if (listeners[i].target_type() == func.target_type()) return true;
        }

        return false;
    }

    void Count()
    {
        return listeners.Size();
    }

    void Empty()
    {
        return listeners.Empty();
    }

private:

    Array<std::function<void(Ts...)>> listeners;

};
