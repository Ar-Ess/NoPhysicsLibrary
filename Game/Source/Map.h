
template<class T, class U>
class Map
{
    struct Node
    {
        Node(T valueA, U valueB)
        {
            this->valueA = valueA;
            this->valueB = valueB;
        }
        T valueA;
        U valueB;
        Node* next = nullptr;
    };
    
public:
    
    void Add(T valueA, U valueB)
    {
        Node* node = new Node(valueA, valueB);
        if (!start) start = node;
        if (final) final->next = node; 
        final = node;
    }
    
    T Get(U value) const
    {
        T ret = T();
        
        Node* node = GetNode(value);
        if (node) ret = node->valueA;
        
        return ret;
    }
    
    U Get(T value) const
    {
        U ret = U();
        
        Node* node = GetNode(value);
        if (node) ret = node->valueB;
        
        return ret;
    }
    
    unsigned int Size() const
    {
        return size;
    }
    
    bool Exists(T value) const
    {
        return GetNode(value) != nullptr;
    }
    
    bool Exists(U value) const
    {
        return GetNode(value) != nullptr;
    }
    
    void Clear()
    {
        Node* node = start;
        for (int i = 0; i < size; ++i)
        {
            if (node->next)
            {
                Node* next = node->next;
                delete node;
                node = nullptr;
                node = next;
            }
            else
            {
                delete node;
                node = nullptr;
            }
        }

        final = nullptr;
        start = nullptr;
        size = 0;
    }
    
private:

    Node* GetNode(T value) const 
    {
        Node* ret = nullptr;
        for (Node* n = start; n != nullptr || n->next != nullptr; n = n->next)
        {
            if (n->valueA == value)
            {
                ret = n;
                break;
            }
        }
        return ret;
    }
    
    Node* GetNode(U value) const 
    {
        Node* ret = nullptr;
        for (Node* n = start; n != nullptr || n->next != nullptr; n = n->next)
        {
            if (n->valueB == value)
            {
                ret = n;
                break;
            }
        }
        return ret;
    }

private:

    Node* start = nullptr;
    Node* final = nullptr;
    unsigned int size = 0;
};
