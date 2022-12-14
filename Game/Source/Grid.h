#pragma once
#include "Vector.h"
typedef unsigned int uint;
template <class T> class Grid
{
    struct Node
    {
        Node(Node* prev, Node* post, T value = nullptr)
        {
            this->value = value;
            this->prev = prev;
            this->post = post;
        }
        Node* prev = nullptr;
        Node* post = nullptr;
        T value = nullptr;
    };

public:

    Grid(uint width, uint height)
    {
        this->width = width;
        this->height = height;
        this->grid = Vector<Vector<T>*>();
        for (uint y = 0; y < height; ++y)
        {
            grid.PushBack(new Vector<T>());
            for (uint x = 0; x < width; ++x) grid.Front()->PushBack(T());
        }
    }

    //bool PushBack(T value)
    //{
    //    Node* node = new Node(value, finalNode, nullptr);
    //    if (finalNode != nullptr) finalNode->post = node;
    //    finalNode = node;
    //    ++size;

    //    if (size <= 1) startNode = node;

    //    return true;
    //}

    //bool PushFront(T value)
    //{
    //    Node* node = new Node(value, nullptr, startNode);
    //    if (startNode != nullptr) startNode->prev = node;
    //    startNode = node;
    //    ++size;

    //    if (size <= 1) finalNode = node;

    //    return true;
    //}

    //bool Insert(T value, unsigned int index)
    //{
    //    if (index >= size) return false;
    //    if (index == 0) return PushFront(value);

    //    Node* post = GetNode(index);
    //    Node* node = new Node(value, post->prev, post);
    //    post->prev->post = node;
    //    post->prev = node;
    //    size++;

    //    return true;
    //}

    //T At(unsigned int index) const
    //{
    //    if (index >= size) return 0;

    //    return GetNode(index)->value;
    //}

    //int Find(T value) const
    //{
    //    if (Empty()) return -1;

    //    int i = 0;
    //    for (Node* node = startNode; node != nullptr; node = node->post)
    //    {
    //        if (node->value == value) return i;
    //        ++i;
    //    }

    //    return -1;
    //}

    //bool Contains(T value) const
    //{
    //    if (Empty()) return false;

    //    for (Node* node = startNode; node != nullptr; node = node->post)
    //    {
    //        if (node->value == value) return true;
    //    }

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

    //    finalNode = nullptr;
    //    startNode = nullptr;
    //    size = 0;

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

    //    --size;
    //    Node* prev = finalNode->prev;
    //    delete finalNode;
    //    finalNode = prev;
    //    prev->post = nullptr;


    //    return true;
    //}

    //bool PopFront()
    //{
    //    if (Empty()) return false;
    //    if (size <= 1) return Clear();

    //    --size;
    //    Node* post = startNode->post;
    //    delete startNode;
    //    startNode = post;
    //    post->prev = nullptr;

    //    return true;
    //}

    //bool Erase(unsigned int index)
    //{
    //    if (index >= size) return false;
    //    if (index == 0) return PopFront();
    //    if (index == size - 1) return PopBack();

    //    Node* node = GetNode(index);
    //    node->prev->post = node->post;
    //    node->post->prev = node->prev;

    //    delete node;
    //    node = nullptr;
    //    --size;

    //    return true;
    //}

    //T operator[](unsigned int index) const
    //{
    //    if (index >= size) return 0;

    //    return GetNode(index)->value;
    //}

private:

    Node* GetNode(unsigned int index) const
    {
        int length = size - 1;
        int medium = (length / 2) + (length % 2);

        Node* node = nullptr;
        if (index - medium < 0)
        {
            node = startNode;
            for (int i = 0; i < index; ++i) node = node->post;
        }
        else
        {
            node = finalNode;
            for (int i = 0; i < (size - index - 1); ++i) node = node->prev;
        }

        return node;
    }

private:

    Vector<Vector<T>*> grid;

    uint width = 0;
    uint height = 0;

};
