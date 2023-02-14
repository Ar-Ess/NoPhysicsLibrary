#pragma once

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

    bool Assign(T value, int index)
    {
        if (index < 0 || index >= size) return false;

        Node* node = GetNode(index);
        node->value = value;

        return true;
    }

    bool Erase(int index)
    {
        if (index < 0 || index >= size) return false;

        Node* erase = GetNode(index);
        if (index == 0)
        {
            start = erase->post;
            delete erase;
            erase = nullptr;
            --size;
            return true;
        }
        if (index == size - 1)
        {
            final = erase->prev;
            delete erase;
            erase = nullptr;
            --size;
            return true;
        }

        erase->prev->post = erase->post;
        erase->post->prev = erase->prev;

        delete erase;
        erase = nullptr;
        --size;

        return true;
    }

    unsigned int Size() const
    {
        return size;
    }

    T At(int index) const
    {
        Node* node = GetNode(index);
        return !node ? 0 : node->value;
    }

    bool Clear()
    {
        if (Empty()) return true;

        Node* node = final;
        for (unsigned int i = 0; i < size; ++i)
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

        return true;
    }

    bool Empty() const
    {
        return size == 0;
    }

    T operator[](int index) const
    {
        Node* node = GetNode(index);
        return !node ? 0 : node->value;
    }

    // Finds the index of a value, returns -1 if not found
    int Find(T value) const
    {
        if (Empty()) return -1;

        unsigned int i = 0;
        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (node->value == value) return i;
            ++i;
        }

        return -1;
    }

    // Swaps the positions of 2 values
    bool Swap(int a, int b)
    {
        if (Empty() || a >= size || a < 0 || b < 0 || b >= size) return false;

        return SwapInternal(a, b);
    }

    // Sorts the array. Send a function that returns a bool.
    //  - Ascendent  sorting: "[](T a, T b) { return a > b; }"
    //  - Descendent sorting: "[](T a, T b) { return a < b; }"
    //  - Matching   sorting: "[](T a, T b) { return a == b;}"
    void Sort(bool(*comparison)(T a, T b))
    {
        this->comparison = comparison;
        if (size > 20) QuickSort(0, size - 1);
        else InsertionSort();

        for (Node* index = start; index->prev != nullptr; index = index->prev) start = index;
        for (Node* index = final; index->post != nullptr; index = index->post) final = index;

        this->comparison = nullptr;
    }

    bool Contains(T value) const
    {
        if (Empty()) return false;

        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (node->value == value) return true;
        }

        return false;
    }

    void Iterate(void(*function)(T value), bool fromStartToEnd = true)
    {
        if (Empty()) return;

        if (fromStartToEnd)
        {
            for (Node* iter = start; iter != nullptr; iter = iter->post)
                function(iter->value);
        }
        else
        {
            for (Node* iter = final; iter != nullptr; iter = iter->prev)
                function(iter->value);
        }
    }

private:

    Node* GetNode(int index) const
    {
        if (index < 0 || index >= size) return nullptr;
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

    bool SwapInternal(unsigned int a, unsigned int b)
    {
        if (a == b) return false;
        if (a > b)
        {
            int ax = a;
            a = b;
            b = ax;
        }

        Node* A = GetNode(a);
        Node* B = GetNode(b);
        bool between = (A->post != B);
        Node* postA = between ? A->post : A;
        Node* prevB = between ? B->prev : B;

        if (A->prev != nullptr) A->prev->post = B;
        B->prev = A->prev;

        if (B->post != nullptr) B->post->prev = A;
        A->post = B->post;

        A->prev = prevB;
        B->post = postA;
        if (between)
        {
            postA->prev = B;
            prevB->post = A;
        }

        // Asing start and end
        if (a == size - 1) final = B;
        else if (a == 0) start = B;
        if (b == size - 1) final = A;
        else if (b == 0) start = A;

        return true;
    }

    void QuickSort(int start, int end)
    {
        if (start >= end) return;

        int index = QSPartition(start, end);

        QuickSort(start, index - 1);
        QuickSort(index + 1, end);
    }

    int QSPartition(int start, int end)
    {
        int i = start;
        Node* pi = GetNode(end);

        for (int j = start; j < end; ++j)
        {
            Node* check = GetNode(j);
            if (comparison(check->value, pi->value))
            {
                SwapInternal(j, i);
                i++;
            }
        }

        SwapInternal(i, end);
        return i;
    }

    bool(*comparison)(T a, T b) = nullptr;

    void InsertionSort()
    {
        for (unsigned int step = 1; step < size; step++)
        {
            int j = step - 1;
            while (j >= 0 && j + 1 < size && !comparison(GetNode(j + 1)->value, GetNode(j)->value))
            {
                SwapInternal(j + 1, j);
                --j;
            }
        }
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

    bool Assign(T* value, int index)
    {
        if (index < 0 || index >= size) return false;

        Node* node = GetNode(index);
        delete node->value;
        node->value = value;

        return true;
    }

    bool Erase(int index)
    {
        if (index < 0 || index >= size) return false;

        Node* erase = GetNode(index);
        if (index == 0)
        {
            start = erase->post;
            delete erase;
            erase = nullptr;
            --size;
            return true;
        }
        if (index == size - 1)
        {
            final = erase->prev;
            delete erase;
            erase = nullptr;
            --size;
            return true;
        }

        erase->prev->post = erase->post;
        erase->post->prev = erase->prev;

        delete erase;
        erase = nullptr;
        --size;

        return true;
    }

    unsigned int Size() const
    {
        return size;
    }

    T* At(int index) const
    {
        Node* node = GetNode(index);
        return !node ? nullptr : node->value;
    }

    bool Clear()
    {
        if (Empty()) return true;

        Node* node = final;
        for (unsigned int i = 0; i < size; ++i)
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

        return true;
    }

    bool Empty() const
    {
        return size == 0;
    }

    T* operator[](int index) const
    {
        Node* node = GetNode(index);
        return !node ? nullptr : node->value;
    }

    // Finds the index of a value, returns -1 if not found
    // Override the operator== of your class for a more acurate search
    int Find(T* value) const
    {
        if (Empty()) return -1;

        unsigned int i = 0;
        for (Node* node = start; node != nullptr; node = node->post)
        {
            // Error? Maybe missing == operand for "ptr variables" or "non-ptr variables"
            if (*node->value == *value) return i;
            ++i;
        }

        return -1;
    }

    // Swaps the positions of 2 values
    bool Swap(int a, int b)
    {
        if (Empty() || a < 0 || a >= size || b < 0 || b >= size) return false;

        return SwapInternal(a, b);
    }

    // Sorts the array. Send a function that returns a bool.
    //  - Ascendent  sorting: "[](T a, T b) { return a > b; }"
    //  - Descendent sorting: "[](T a, T b) { return a < b; }"
    //  - Matching   sorting: "[](T a, T b) { return a == b;}"
    void Sort(bool(*comparison)(T* a, T* b))
    {
        this->comparison = comparison;
        if (size > 20) QuickSort(0, size - 1);
        else InsertionSort();

        for (Node* index = start; index->prev != nullptr; index = index->prev) start = index;
        for (Node* index = final; index->post != nullptr; index = index->post) final = index;

        this->comparison = nullptr;
    }

    bool Contains(T* value) const
    {
        if (Empty()) return false;

        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (node->value == value) return true;
        }

        return false;
    }

    void Iterate(void(*function)(T* value), bool fromStartToEnd = true)
    {
        if (Empty()) return;

        if (fromStartToEnd)
        {
            for (Node* iter = start; iter != nullptr; iter = iter->post)
                function(iter->value);
        }
        else
        {
            for (Node* iter = final; iter != nullptr; iter = iter->prev)
                function(iter->value);
        }
    }

private:

    Node* GetNode(int index) const
    {
        if (index < 0 || index >= size) return nullptr;
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

    bool SwapInternal(unsigned int a, unsigned int b)
    {
        if (a == b) return false;
        if (a > b)
        {
            int ax = a;
            a = b;
            b = ax;
        }

        Node* A = GetNode(a);
        Node* B = GetNode(b);
        bool between = (A->post != B);
        Node* postA = between ? A->post : A;
        Node* prevB = between ? B->prev : B;

        if (A->prev != nullptr) A->prev->post = B;
        B->prev = A->prev;

        if (B->post != nullptr) B->post->prev = A;
        A->post = B->post;

        A->prev = prevB;
        B->post = postA;
        if (between)
        {
            postA->prev = B;
            prevB->post = A;
        }

        // Asing start and end
        if (a == size - 1) final = B;
        else if (a == 0) start = B;
        if (b == size - 1) final = A;
        else if (b == 0) start = A;

        return true;
    }

    void QuickSort(int start, int end)
    {
        if (start >= end) return;

        int index = QSPartition(start, end);

        QuickSort(start, index - 1);
        QuickSort(index + 1, end);
    }

    int QSPartition(int start, int end)
    {
        int i = start;
        Node* pi = GetNode(end);

        for (int j = start; j < end; ++j)
        {
            Node* check = GetNode(j);
            if (comparison(check->value, pi->value))
            {
                SwapInternal(j, i);
                i++;
            }
        }

        SwapInternal(i, end);
        return i;
    }

    bool(*comparison)(T* a, T* b) = nullptr;

    void InsertionSort()
    {
        for (unsigned int step = 1; step < size; step++)
        {
            int j = step - 1;
            while (j >= 0 && j + 1 < size && !comparison(GetNode(j + 1)->value, GetNode(j)->value))
            {
                SwapInternal(j + 1, j);
                --j;
            }
        }
    }

private:

    Node* start = nullptr;
    Node* final = nullptr;
    unsigned int size = 0;

};
