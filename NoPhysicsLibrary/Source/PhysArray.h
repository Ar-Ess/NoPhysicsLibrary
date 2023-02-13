#pragma once
#include <assert.h>

template<class T>
class PhysArray
{
    struct Node
    {
        Node(T value, Node* prev)
        {
            this->value = value;
            this->prev = prev;
            this->post = nullptr;
        }
        T value;
        Node* prev = nullptr;
        Node* post = nullptr;
    };
    
public:

    void Add(T value)
    {
        Node* node = new Node(value, final);
        if (!start) start = node;
        if (final) final->post = node;
        final = node;
        ++size;
    }

    bool Assign(T value, unsigned int index)
    {
        if (index >= size) return false;

        Node* node = GetNode(index);
        node->value = value;

        return true;
    }
    
    void Erase(unsigned int index)
    {
        if (index >= size) return;
        Node* erase = GetNode(index);
        if (index == 0)
        {
            start = erase->post;
            delete erase;
            erase = nullptr;
            --size;
            return;
        }
        if (index == size - 1)
        {
            final = erase->prev;
            delete erase;
            erase = nullptr;
            --size;
            return;
        }

        erase->prev->post = erase->post;
        erase->post->prev = erase->prev;
        delete erase;
        erase = nullptr;
        --size;
    }
    
    unsigned int Size() const
    {
        return size;
    }
    
    T At(unsigned int index) const
    {
        return GetNode(index)->value;
    }
    
    void Clear()
    {
        Node* node = final;
        for (int i = 0; i < size; ++i)
        {
            if (node->post)
            {
                delete node->post;
                node->post = nullptr;
            }
            if (node->prev) node = node->prev;
        }
        delete node;
        node = nullptr;

        final = nullptr;
        start = nullptr;
        size = 0;
    }

    bool Empty() const
    {
        return size == 0;
    }

    T operator[](unsigned int index) const
    {
        if (index >= size) return 0;

        return GetNode(index)->value;
    }

    // Finds the index of a value, returns -1 if not found
    int Find(T value) const
    {
        if (Empty()) return -1;

        int i = 0;
        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (node->value == value) return i;
            ++i;
        }

        return -1;
    }
    
private:

    Node* GetNode(unsigned int index) const
    {
	    if (index >= size) return nullptr;
        if (index == 0) return start;
        if (index == size - 1) return final;
        unsigned int middle = size / 2 + size % 2;
        
        Node* ret = nullptr;
        if (index <= middle - 1)
        {
            ret = start;
            for (unsigned int i = 0; i < index; ++i) ret = ret->post;
        }
        else
        {
            ret = final;
            for (unsigned int i = 0; i < size - index - 1; ++i) ret = ret->prev;
        }
        
        return ret;
    }

private:

    Node* start = nullptr;
    Node* final = nullptr;
    unsigned int size = 0;

};

template<class T>
class PhysArray<T*>
{
    struct Node
    {
        Node(T* value, Node* prev)
        {
            this->value = value;
            this->prev = prev;
            this->post = nullptr;
        }
        ~Node()
        {
            delete value;
            value = nullptr;
        }
        T* value;
        Node* prev = nullptr;
        Node* post = nullptr;
    };

public:

    void Add(T* value)
    {
        Node* node = new Node(value, final);
        if (!start) start = node;
        if (final) final->post = node;
        final = node;
        ++size;
    }

    bool Assign(T* value, unsigned int index)
    {
        if (index >= size) return false;

        Node* node = GetNode(index);
        delete node->value;
        node->value = value;

        return true;
    }

    void Erase(unsigned int index)
    {
        if (index >= size) return;
        Node* erase = GetNode(index);
        if (index == 0)
        {
            start = erase->post;
            delete erase;
            erase = nullptr;
            --size;
            return;
        }
        if (index == size - 1)
        {
            final = erase->prev;
            delete erase;
            erase = nullptr;
            --size;
            return;
        }

        erase->prev->post = erase->post;
        erase->post->prev = erase->prev;
        delete erase;
        erase = nullptr;
        --size;
    }

    unsigned int Size() const
    {
        return size;
    }

    T* At(unsigned int index) const
    {
        return GetNode(index)->value;
    }

    void Clear()
    {
        Node* node = final;
        for (int i = 0; i < size; ++i)
        {
            if (node->post)
            {
                delete node->post;
                node->post = nullptr;
            }
            if (node->prev) node = node->prev;
        }
        delete node;
        node = nullptr;

        final = nullptr;
        start = nullptr;
        size = 0;
    }

    bool Empty() const
    {
        return size == 0;
    }

    T* operator[](unsigned int index) const
    {
        assert(index < size);

        return GetNode(index)->value;
    }

    // Finds the index of a value, returns -1 if not found
    int Find(T* value) const
    {
        if (Empty()) return -1;

        int i = 0;
        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (*node->value == *value) return i;
            ++i;
        }

        return -1;
    }

private:

    Node* GetNode(unsigned int index) const
    {
        assert(index < size);
        if (index == 0) return start;
        if (index == size - 1) return final;
        uint middle = size / 2 + size % 2;

        Node* ret = nullptr;
        if (index <= middle - 1)
        {
            ret = start;
            for (uint i = 0; i < index; ++i) ret = ret->post;
        }
        else
        {
            ret = final;
            for (uint i = 0; i < size - index - 1; ++i) ret = ret->prev;
        }

        return ret;
    }

private:

    Node* start = nullptr;
    Node* final = nullptr;
    unsigned int size = 0;

};
