#pragma once
#include "Array.h"
#include "Flag.h"
typedef unsigned int uint;
template <class T>
class Grid
{
    struct Row
    {
        Row(uint width)
        {
            uint flagAmount = (uint)ceil(width / 8.0f);
            for (uint i = 0; i < flagAmount; ++i) full.Add(new Flag());
        }
        void SetValue(T value, uint x)
        {
            SetFullFlag(x, true);
            row.Assign(value, x);
        }
        void SetFullFlag(uint x, bool state)
        {
            uint setIndex = x / 8;
            uint flagIndex = x - 8 * setIndex;
            full.At(setIndex)->Set(flagIndex, state);
        }
        bool Empty(uint x)
        {
            uint setIndex = x / 8;
            uint flagIndex = x - 8 * setIndex;
            return !full.At(setIndex)->Get(flagIndex);
        }
        bool Empty()
        {
            return size == 0;
        }
        T GetNonEmptyValue(uint x, uint width)
        {
            uint fullCells = 0;
            for (uint i = 0; i < width; ++i)
            {
                if (Empty(i)) continue;

                ++fullCells;
                if (x == fullCells - 1) return row.At(i);
            }

            return T();
        }
        Array<T> row;
        Array<Flag*> full;
        uint size = 0;
    };

public:

    //-TODO: make grid dynamic possible
    Grid(uint width, uint height)
    {
        this->width = width;
        this->height = height;
        this->grid = Array<Row*>();
        for (uint y = 0; y < height; ++y)
        {
            grid.Add(new Row(width));
            for (uint x = 0; x < width; ++x) grid.At(y)->row.Add(T());
        }
    }

    bool Set(T value, uint x, uint y)
    {
        if (x >= width && y >= height) return false;

        Row* row = GetRow(y);
        row->size++;

        row->SetValue(value, x);
        size++;
    }

    T At(uint x, uint y) const
    {
        assert(x < width && y < height);

        return grid.At(y)->row.At(x);
    }

    // Returns the amount of not empty nodes
    uint Size() const
    {
        return size;
    }

    // Returns the total amount of nodes
    uint SizeMax() const
    {
        return width * height;
    }

    uint Width() const
    {
        return width;
    }

    uint Heigth() const
    {
        return height;
    }

    // Returns the amount of not empty nodes in a row
    uint SizeRow(uint row) const
    {
        if (row >= width) return 0;

        return grid.At(row)->size;
    }

    bool Empty() const
    {
        return (size <= 0);
    }

    bool Empty(uint x, uint y) const
    {
        return GetRow(y)->Empty(x);
    }

    // Returns the not empty cells sequentially
    // Optimized to iterate
    T operator[](uint index)
    {
        assert(index < size);
        assert(size > 0);
        const uint offset = index / width;

        for (uint i = 0; i < height; ++i)
        {
            Row* row = GetRow(i);
            if (i >= offset && int(index) - int(row->size) < 0) return row->GetNonEmptyValue(index, width);
            index -= row->size;
        }

        //uint rowIndex = index / width;
        //int indx = index - (width * rowIndex);
        //for (uint i = rowIndexº; i < height; ++i)
        //{
        //    Row* row = GetRow(i);
        //    if (row->Empty()) continue;
        //    if (row->size - 1 >= indx) return row->GetNonEmptyValue(indx, width);
        //    indx -= row->size;
        //}

        return T();

    }

    //bool PushBack(T value)
    //{
    //    Node* node = new Node(value, finalNode, nullptr);
    //    if (finalNode != nullptr) finalNode->post = node;
    //    finalNode = node;
    //    ++size;
    //
    //    if (size <= 1) startNode = node;
    //
    //    return true;
    //}

    //bool PushFront(T value)
    //{
    //    Node* node = new Node(value, nullptr, startNode);
    //    if (startNode != nullptr) startNode->prev = node;
    //    startNode = node;
    //    ++size;
    //
    //    if (size <= 1) finalNode = node;
    //
    //    return true;
    //}

    //bool Insert(T value, unsigned int index)
    //{
    //    if (index >= size) return false;
    //    if (index == 0) return PushFront(value);
    //
    //    Node* post = GetNode(index);
    //    Node* node = new Node(value, post->prev, post);
    //    post->prev->post = node;
    //    post->prev = node;
    //    size++;
    //
    //    return true;
    //}

    //int Find(T value) const
    //{
    //    if (Empty()) return -1;
    // 
    //    int i = 0;
    //    for (Node* node = startNode; node != nullptr; node = node->post)
    //    {
    //        if (node->value == value) return i;
    //        ++i;
    //    }
    // 
    //    return -1;
    //}

    //bool Contains(T value) const
    //{
    //    if (Empty()) return false;
    // 
    //    for (Node* node = startNode; node != nullptr; node = node->post)
    //    {
    //        if (node->value == value) return true;
    //    }
    // 
    //    return false;
    //}

    //unsigned int Size() const
    //{
    //    return width * height;
    //}

    //unsigned int Width() const
    //{
    //    return width;
    //}

    //unsigned int Height() const
    //{
    //    return height;
    //}

    //bool Clear()
    //{
    //    Node* node = finalNode;
    //    for (int i = 0; i < size; ++i)
    //    {
    //        if (node->post)
    //        {
    //            delete node->post;
    //            node->post = nullptr;
    //        }
    //        if (node->prev) node = node->prev;
    //    }
    //    delete node;
    //    node = nullptr;
    // 
    //    finalNode = nullptr;
    //    startNode = nullptr;
    //    size = 0;
    // 
    //    return true;
    //}

    //T Front() const
    //{
    //    return startNode->value;
    //}

    //T Back() const
    //{
    //    return finalNode->value;
    //}

    //bool Empty() const
    //{
    //    return size == 0;
    //}

    //bool PopBack()
    //{
    //    if (Empty()) return false;
    //    if (size <= 1) return Clear();
    // 
    //    --size;
    //    Node* prev = finalNode->prev;
    //    delete finalNode;
    //    finalNode = prev;
    //    prev->post = nullptr;
    // 
    // 
    //    return true;
    //}

    //bool PopFront()
    //{
    //    if (Empty()) return false;
    //    if (size <= 1) return Clear();
    // 
    //    --size;
    //    Node* post = startNode->post;
    //    delete startNode;
    //    startNode = post;
    //    post->prev = nullptr;
    // 
    //    return true;
    //}

    //bool Erase(unsigned int index)
    //{
    //    if (index >= size) return false;
    //    if (index == 0) return PopFront();
    //    if (index == size - 1) return PopBack();
    // 
    //    Node* node = GetNode(index);
    //    node->prev->post = node->post;
    //    node->post->prev = node->prev;
    // 
    //    delete node;
    //    node = nullptr;
    //    --size;
    // 
    //    return true;
    //}

    //T operator[](unsigned int index) const
    //{
    //    if (index >= size) return 0;
    // 
    //    return GetNode(index)->value;
    //}

private:

    Row* GetRow(uint y) const
    {
        return grid.At(y);
    }

private:

    Array<Row*> grid;
    Array<T*> flatGrid;

    uint width = 0;
    uint height = 0;
    uint size = 0;

};

//template <class T>
//class Grid<T*>
//{
//
//    struct Value
//    {
//        Value(T* value = nullptr) { this->value = value; }
//        ~Value() { delete value; }
//        T* value = nullptr;
//    };
//
//public:
//
//    //-TODO: make grid dynamic possible
//    Grid(uint width, uint height, bool dynamic = false)
//    {
//        dynamic = false;
//        this->dynamic = dynamic;
//        this->width = width;
//        this->height = height;
//        this->grid = Vector<Vector<Value*>*>();
//        for (uint y = 0; y < height; ++y)
//        {
//            grid.PushBack(new Vector<Value*>());
//            for (uint x = 0; x < width; ++x) grid.Back()->PushBack(new Value());
//        }
//    }
//
//    bool Add(T* value, uint x, uint y)
//    {
//        if (x >= width && !dynamic) return false;
//        if (y >= height && !dynamic) return false;
//
//        Value* v = grid.At(y)->At(x);
//        delete v->value;
//        v->value = value;
//    }
//
//    T* At(uint x, uint y) const
//    {
//        assert(x < width || dynamic);
//        assert(y < height || dynamic);
//
//        return grid.At(y)->At(x)->value;
//    }
//
//    //bool PushBack(T value)
//    //{
//    //    Node* node = new Node(value, finalNode, nullptr);
//    //    if (finalNode != nullptr) finalNode->post = node;
//    //    finalNode = node;
//    //    ++size;
//    //
//    //    if (size <= 1) startNode = node;
//    //
//    //    return true;
//    //}
//
//    //bool PushFront(T value)
//    //{
//    //    Node* node = new Node(value, nullptr, startNode);
//    //    if (startNode != nullptr) startNode->prev = node;
//    //    startNode = node;
//    //    ++size;
//    //
//    //    if (size <= 1) finalNode = node;
//    //
//    //    return true;
//    //}
//
//    //bool Insert(T value, unsigned int index)
//    //{
//    //    if (index >= size) return false;
//    //    if (index == 0) return PushFront(value);
//    //
//    //    Node* post = GetNode(index);
//    //    Node* node = new Node(value, post->prev, post);
//    //    post->prev->post = node;
//    //    post->prev = node;
//    //    size++;
//    //
//    //    return true;
//    //}
//
//    //int Find(T value) const
//    //{
//    //    if (Empty()) return -1;
//    // 
//    //    int i = 0;
//    //    for (Node* node = startNode; node != nullptr; node = node->post)
//    //    {
//    //        if (node->value == value) return i;
//    //        ++i;
//    //    }
//    // 
//    //    return -1;
//    //}
//
//    //bool Contains(T value) const
//    //{
//    //    if (Empty()) return false;
//    // 
//    //    for (Node* node = startNode; node != nullptr; node = node->post)
//    //    {
//    //        if (node->value == value) return true;
//    //    }
//    // 
//    //    return false;
//    //}
//
//    //unsigned int Size() const
//    //{
//    //    return width * height;
//    //}
//
//    //unsigned int Width() const
//    //{
//    //    return width;
//    //}
//
//    //unsigned int Height() const
//    //{
//    //    return height;
//    //}
//
//    //bool Clear()
//    //{
//    //    Node* node = finalNode;
//    //    for (int i = 0; i < size; ++i)
//    //    {
//    //        if (node->post)
//    //        {
//    //            delete node->post;
//    //            node->post = nullptr;
//    //        }
//    //        if (node->prev) node = node->prev;
//    //    }
//    //    delete node;
//    //    node = nullptr;
//    // 
//    //    finalNode = nullptr;
//    //    startNode = nullptr;
//    //    size = 0;
//    // 
//    //    return true;
//    //}
//
//    //T Front() const
//    //{
//    //    return startNode->value;
//    //}
//
//    //T Back() const
//    //{
//    //    return finalNode->value;
//    //}
//
//    //bool Empty() const
//    //{
//    //    return size == 0;
//    //}
//
//    //bool PopBack()
//    //{
//    //    if (Empty()) return false;
//    //    if (size <= 1) return Clear();
//    // 
//    //    --size;
//    //    Node* prev = finalNode->prev;
//    //    delete finalNode;
//    //    finalNode = prev;
//    //    prev->post = nullptr;
//    // 
//    // 
//    //    return true;
//    //}
//
//    //bool PopFront()
//    //{
//    //    if (Empty()) return false;
//    //    if (size <= 1) return Clear();
//    // 
//    //    --size;
//    //    Node* post = startNode->post;
//    //    delete startNode;
//    //    startNode = post;
//    //    post->prev = nullptr;
//    // 
//    //    return true;
//    //}
//
//    //bool Erase(unsigned int index)
//    //{
//    //    if (index >= size) return false;
//    //    if (index == 0) return PopFront();
//    //    if (index == size - 1) return PopBack();
//    // 
//    //    Node* node = GetNode(index);
//    //    node->prev->post = node->post;
//    //    node->post->prev = node->prev;
//    // 
//    //    delete node;
//    //    node = nullptr;
//    //    --size;
//    // 
//    //    return true;
//    //}
//
//    //T operator[](unsigned int index) const
//    //{
//    //    if (index >= size) return 0;
//    // 
//    //    return GetNode(index)->value;
//    //}
//
//private:
//
//    Array<Array<Value*>*> grid;
//
//    uint width = 0;
//    uint height = 0;
//    bool dynamic = false;
//
//};
