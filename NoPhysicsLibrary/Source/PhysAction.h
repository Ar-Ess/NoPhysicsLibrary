#pragma once
#include "PhysArray.h"
#include <functional>

template<class ...Ts>
class PhysAction
{

    friend class NPL;

public:

    void Invoke(Ts... value)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
            listeners[i](value...);
    }

private:

    PhysAction() {}

    ~PhysAction() { listeners.Clear(); }

    // For global functions:
    //      operator+= std::bind(Func, args);
    // For member functions:
    //      operator+= std::bind(&Class::Func, PtrToClass, args);
    // For future arguments inputed, use std::placeholders
    //      operaror+= std::bind(&Class::Func, PtrToClass, placeholder)
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

private:

    PhysArray<std::function<void(Ts...)>> listeners;

};
