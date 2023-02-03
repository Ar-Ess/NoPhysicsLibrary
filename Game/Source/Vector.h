#pragma once

template <class T>
class Vector
{
    struct Node
    {
        Node(T value, Node* prev, Node* post)
        {
            this->value = value;
            this->prev = prev;
            this->post = post;
        }
        Node* prev = nullptr;
        Node* post = nullptr;
        T value;
    };

public:

    bool PushBack(T value)
    {
        Node* node = new Node(value, final, nullptr);
        if (final != nullptr) final->post = node;
        final = node;
        ++size;

        if (size <= 1) start = node;

        return true;
    }

    bool PushFront(T value)
    {
        Node* node = new Node(value, nullptr, start);
        if (start != nullptr) start->prev = node;
        start = node;
        ++size;

        if (size <= 1) final = node;

        return true;
    }

    bool Insert(T value, unsigned int index)
    {
        if (index >= size) return false;
        if (index == 0) return PushFront(value);

        Node* post = GetNode(index);
        Node* node = new Node(value, post->prev, post);
        post->prev->post = node;
        post->prev = node;
        size++;

        return true;
    }

    T At(unsigned int index) const
    {
        Node* node = GetNode(index);
        return !node ? 0 : node->value;
    }

    bool Assign(T value, unsigned int index)
    {
        if (index >= size) return false;

        Node* node = GetNode(index);
        node->value = value;

        return true;
    }

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

    bool Contains(T value) const
    {
        if (Empty()) return false;

        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (node->value == value) return true;
        }

        return false;
    }

    unsigned int Size() const
    {
        return size;
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

    T Front() const
    {
        return start->value;
    }

    T Back() const
    {
        return final->value;
    }

    bool Empty() const
    {
        return size == 0;
    }

    bool PopBack()
    {
        if (Empty()) return false;
        if (size <= 1) return Clear();

        --size;
        Node* prev = final->prev;
        delete final;
        final = prev;
        prev->post = nullptr;


        return true;
    }

    bool PopFront()
    {
        if (Empty()) return false;
        if (size <= 1) return Clear();

        --size;
        Node* post = start->post;
        delete start;
        start = post;
        post->prev = nullptr;

        return true;
    }

    bool Erase(unsigned int index)
    {
        if (index >= size) return false;
        if (index == 0) return PopFront();
        if (index == size - 1) return PopBack();

        Node* node = GetNode(index);
        node->prev->post = node->post;
        node->post->prev = node->prev;

        delete node;
        node = nullptr;
        --size;

        return true;
    }

    T operator[](unsigned int index) const
    {
        Node* node = GetNode(index);
        return !node ? 0 : node->value;
    }

    void Reverse()
    {
        if (Size() <= 1) return;

        for (Node* node = start; node != nullptr; node = node->prev)
        {
            Node* prev = node->prev;
            node->prev = node->post;
            node->post = prev;
        }

        Node* start = start;
        start = final;
        final = start;
    }

    // Swaps the positions of 2 values
    bool Swap(unsigned int a, unsigned int b)
    {
        if (Empty() || a >= size || b >= size) return false;

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

private:

    Node* GetNode(unsigned int index) const
    {
        if (index >= size) return nullptr;
        if (index == 0) return start;
        if (index == size - 1) return final;
        unsigned int middle = size / 2 + size % 2;

        Node* ret = nullptr;
        if (index - medium < 0)
        {
            ret = start;
            for (unsigned int i = 0; i < index; ++i) ret = ret->post;
        }
        else
        {
            ret = final;
            for (unsigned int i = 0; i < (size - index - 1); ++i) ret = ret->prev;
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

template <class T>
class Vector<T*>
{
    struct Node
    {
        Node(T* value, Node* prev, Node* post)
        {
            this->value = value;
            this->prev = prev;
            this->post = post;
        }
        ~Node()
        {
            delete value;
            value = nullptr;
        }
        Node* prev = nullptr;
        Node* post = nullptr;
        T* value;
    };

public:

    bool PushBack(T* value)
    {
        Node* node = new Node(value, finalNode, nullptr);
        if (finalNode != nullptr) finalNode->post = node;
        finalNode = node;
        ++size;

        if (size <= 1) start = node;

        return true;
    }

    bool PushFront(T* value)
    {
        Node* node = new Node(value, nullptr, start);
        if (start != nullptr) start->prev = node;
        start = node;
        ++size;

        if (size <= 1) finalNode = node;

        return true;
    }

    bool Insert(T* value, unsigned int index)
    {
        if (index >= size) return false;
        if (index == 0) return PushFront(value);

        Node* post = GetNode(index);
        Node* node = new Node(value, post->prev, post);
        post->prev->post = node;
        post->prev = node;
        size++;

        return true;
    }

    T* At(unsigned int index) const
    {
        Node* node = GetNode(index);
        return !node ? nullptr : node->value;
    }

    bool Assign(T* value, unsigned int index)
    {
        if (index >= size) return false;

        Node* node = GetNode(index);
        delete node->value;
        node->value = value;

        return true;
    }

    int Find(T* value) const
    {
        if (Empty()) return -1;

        unsigned int i = 0;
        for (Node* node = start; node != nullptr; node = node->post)
        {
            if (*node->value == *value) return i;
            ++i;
        }

        return -1;
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

    unsigned int Size() const
    {
        return size;
    }

    bool Clear()
    {
        Node* node = finalNode;
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

        finalNode = nullptr;
        start = nullptr;
        size = 0;

        return true;
    }

    T* Front() const
    {
        return start->value;
    }

    T* Back() const
    {
        return finalNode->value;
    }

    bool Empty() const
    {
        return size == 0;
    }

    bool PopBack()
    {
        if (Empty()) return false;
        if (size <= 1) return Clear();

        --size;
        Node* prev = finalNode->prev;
        delete finalNode;
        finalNode = prev;
        prev->post = nullptr;


        return true;
    }

    bool PopFront()
    {
        if (Empty()) return false;
        if (size <= 1) return Clear();

        --size;
        Node* post = start->post;
        delete start;
        start = post;
        post->prev = nullptr;

        return true;
    }

    bool Erase(unsigned int index)
    {
        if (index >= size) return false;
        if (index == 0) return PopFront();
        if (index == size - 1) return PopBack();

        Node* node = GetNode(index);
        node->prev->post = node->post;
        node->post->prev = node->prev;

        delete node;
        node = nullptr;
        --size;

        return true;
    }

    T* operator[](unsigned int index) const
    {
        Node* node = GetNode(index);
        return !node ? nullptr : node->value;
    }

    void Reverse()
    {
        if (Size() <= 1) return;

        for (Node* node = start; node != nullptr; node = node->prev)
        {
            Node* prev = node->prev;
            node->prev = node->post;
            node->post = prev;
        }

        Node* start = start;
        start = finalNode;
        finalNode = start;
    }

    // Swaps the positions of 2 values
    bool Swap(unsigned int a, unsigned int b)
    {
        if (Empty() || a >= size || b >= size) return false;

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

private:

    Node* GetNode(unsigned int index) const
    {
        if (index >= size) return nullptr;
        if (index == 0) return start;
        if (index == size - 1) return final;
        unsigned int middle = size / 2 + size % 2;

        Node* ret = nullptr;
        if (index - medium < 0)
        {
            ret = start;
            for (unsigned int i = 0; i < index; ++i) ret = ret->post;
        }
        else
        {
            ret = final;
            for (unsigned int i = 0; i < (size - index - 1); ++i) ret = ret->prev;
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
    Node* finalNode = nullptr;
    unsigned int size = 0;

};
