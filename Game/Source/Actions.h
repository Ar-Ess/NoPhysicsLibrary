#pragma once
#include "Array.h"

template<class ...Ts>
class Action
{
public:

    Action() {}
    
    ~Action() {}

    void Invoke(Ts... value)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
            (*listeners[i])(value...);
    }

    void operator+=(void(*func)(Ts...))
    {
        listeners.Add(func);
    }

    void operator-=(void(*func)(Ts...))
    {
        if (listeners.Empty()) return;

        int index = listeners.Find(func);
        if (index != -1) listeners.Erase(index);
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

    Array<void(*)(Ts...)> listeners;

};

template<class T, class ...Ts>
class CAction
{
public:

    CAction() {}

    ~CAction() {}

    void Invoke(Ts... value, T provider)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
            (provider.*listeners[i])(value...);
    }

    void Invoke(Ts... value, T* provider)
    {
        if (listeners.Empty()) return;

        for (int i = 0; i < listeners.Size(); ++i)
            (provider->*listeners[i])(value...);
    }

    void operator+=(void(T::*func)(Ts...))
    {
        listeners.Add(func);
    }

    void operator-=(void(T::*func)(Ts...))
    {
        if (listeners.Empty()) return;

        int index = listeners.Find(func);
        if (index != -1) listeners.Erase(index);
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

    Array<void(T::*)(Ts...)> listeners;

};
