#pragma once

template<class T>
class Array
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
        //-TODO: delete previous value
        node->value = value;

        return true;
    }
    
    void Erase(uint index)
    {
	if (index >= size) return nullptr;
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
    
    uint Size() const
    {
        return size;
    }
    
    T At(uint index) const
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
    
private:

    Node* GetNode(uint index) const
    {
	    if (index >= size) return nullptr;
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
    uint size = 0;

};
