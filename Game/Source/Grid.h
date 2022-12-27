#pragma once
#include "Array.h"
#include "Flag.h"
#include "Point.h"

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
        void Clear()
        {
            size = 0;
            for (uint i = 0; i < full.Size(); ++i) full[i]->Clear();
        }

        Array<T> row;
        Array<Flag*> full;
        uint size = 0;
    };

public:

    Grid(uint width, uint height)
    {
        this->width = width;
        this->height = height;
        this->grid = new Array<Row*>();
        for (uint y = 0; y < height; ++y)
        {
            grid->Add(new Row(width));
            for (uint x = 0; x < width; ++x) grid->At(y)->row.Add(T());
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
    
    bool Set(T value, Point coords)
    {
        if (coords.x >= width && coords.y >= height) return false;

        Row* row = GetRow(coords.y);
        row->size++;

        row->SetValue(value, coords.x);
        size++;
    }

    T At(uint x, uint y) const
    {
        assert(x < width && y < height);

        return GetRow(y)->row.At(x);
    }

    T At(Point coords) const
    {
        assert(coords.x < width&& coords.y < height);

        return GetRow(coords.y)->row.At(coords.x);
    }

    T At(uint index) const
    {
        assert(index < width * height);
        Point coords = FromIndexToCoords(index);

        return GetRow(coords.y)->row.At(coords.x);
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

        return GetRow(row)->size;
    }

    bool Empty() const
    {
        return (size <= 0);
    }

    bool Empty(uint x, uint y) const
    {
        assert(x < width && y < height);
        return GetRow(y)->Empty(x);
    }

    bool Empty(Point coords) const
    {
        assert(x < width&& y < height);
        return GetRow(coords.y)->Empty(coords.x);
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

        return T();
    }

    Point Find(T value) const
    {
        assert(!Empty());

        Point ret = {-1, -1};
        for (uint i = 0; i < height; ++i)
        {
            Row* row = GetRow(i);
            if (row->Empty()) continue;
            int index = row->Find(value);
            if (index == -1) continue;
            ret = {index, i};
            break;
        }

        return ret;
    }

    // Empty all cells
    bool Clear()
    {
        if (Empty()) return true;

        for (uint i = 0; i < height; ++i) GetRow(i)->Clear();
        size = 0;

        return true;
    }

    // Clean the structure. Return to a 0 by 0 grid
    bool CleanUp()
    {
        for (uint i = 0; i < height: ++i)
        {
            Row* row = GetRow(i);
            row->full->Clear();
            row->row->Clear();
            size = 0;
        }
        grid->Clear();
        delete grid;
        grid = nullptr;

        this->width = 0;
        this->height = 0;
        this->size = 0;

        return true;
    }

    bool Erase(uint x, uint y)
    {
        assert(x < width && y < height);

        Row* row = GetRow(y);
        row->size--;
        row->SetFullFlag(x, false);

        return true;
    }
    
    bool Erase(Point coords)
    {
        assert(coords.x < width && coords.y < height);

        Row* row = GetRow(coords.y);
        row->size--;
        row->SetFullFlag(coords.x, false);

        return true;
    }

    bool Erase(uint index)
    {
        assert(index < width * height);
        Point coords = FromIndexToCoords(index);
        Row* row = GetRow(coords.y);

        row->size--;
        row->SetFullFlag(coords.x, false);

        return true;
    }

private:

    Row* GetRow(uint y) const
    {
        return grid->At(y);
    }

    Point FromIndexToCoords(uint index)
    {
        uint y = index / width;
        uint x = index - width * y;

        return {x, y};
    }

private:

    Array<Row*>* grid = nullptr;

    uint width = 0;
    uint height = 0;
    uint size = 0;

};

template <class T>
class Grid<T*>
{
    struct Row
    {
        void SetValue(T* value, uint x)
        {
            row.Assign(value, x);
        }
        bool Empty(uint x)
        {
            return row.At(x) == nullptr;
        }
        bool Empty()
        {
            return size == 0;
        }
        T* GetNonEmptyValue(uint x, uint width)
        {
            uint fullCells = 0;
            for (uint i = 0; i < width; ++i)
            {
                if (Empty(i)) continue;

                ++fullCells;
                if (x == fullCells - 1) return row.At(i);
            }

            return nullptr;
        }
        void Clear()
        {
            size = 0;
            for (uint i = 0; i < row->Size(); ++i)
            {
                T* value = row[i];
                delete value;
                value = nullptr;
            }
        }

        Array<T*> row;
        uint size = 0;
    };

public:

    Grid(uint width, uint height)
    {
        this->width = width;
        this->height = height;
        this->grid = new Array<Row*>();
        for (uint y = 0; y < height; ++y)
        {
            grid->Add(new Row());
            for (uint x = 0; x < width; ++x) grid->At(y)->row.Add(nullptr);
        }
    }

    bool Set(T* value, uint x, uint y)
    {
        if (x >= width && y >= height) return false;

        Row* row = GetRow(y);
        row->size++;

        row->SetValue(value, x);
        size++;
    }

    bool Set(T* value, Point coords)
    {
        if (coords.x >= width && coords.y >= height) return false;

        Row* row = GetRow(coords.y);
        row->size++;

        row->SetValue(value, coords.x);
        size++;
    }

    bool Set(T* value, uint index)
    {
        if (index < width * height) return false;

        Point coords = FromIndexToCoords(index);

        Row* row = GetRow(coords.y);
        row->size++;

        row->SetValue(value, coords.x);
        size++;
    }

    T* At(uint x, uint y) const
    {
        assert(x < width&& y < height);

        return GetRow(y)->row.At(x);
    }

    T* At(Point coords) const
    {
        assert(coords.x < width&& coords.y < height);

        return GetRow(coords.y)->row.At(coords.x);
    }

    T* At(uint index) const
    {
        assert(index < width* height);
        Point coords = FromIndexToCoords(index);

        return GetRow(coords.y)->row.At(coords.x);
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

        return GetRow(row)->size;
    }

    bool Empty() const
    {
        return (size <= 0);
    }

    bool Empty(uint x, uint y) const
    {
        assert(x < width&& y < height);
        return GetRow(y)->Empty(x);
    }

    bool Empty(Point coords) const
    {
        assert(x < width&& y < height);
        return GetRow(coords.y)->Empty(coords.x);
    }

    Point Find(T* value) const
    {
        assert(!Empty());

        Point ret = { -1, -1 };
        for (uint i = 0; i < height; ++i)
        {
            Row* row = GetRow(i);
            if (row->Empty()) continue;
            int index = row->Find(value);
            if (index == -1) continue;
            ret = { index, i };
            break;
        }

        return ret;
    }

    // Empty all cells
    bool Clear()
    {
        if (Empty()) return true;

        for (uint i = 0; i < height; ++i) GetRow(i)->Clear();
        size = 0;

        return true;
    }

    // Clean the structure. Return to a 0 by 0 grid
    bool CleanUp()
    {
        for (uint i = 0; i < height: ++i)
        {
            Row* row = GetRow(i);
            row->row->Clear();
        }
        grid->Clear();
        delete grid;
        grid = nullptr;

        this->width = 0;
        this->height = 0;
        this->size = 0;

        return true;
    }

    bool Erase(uint x, uint y)
    {
        assert(x < width&& y < height);

        Row* row = GetRow(y);
        row->size--;
        T* value = row[x];
        delete value;
        value = nullptr;

        return true;
    }

    bool Erase(Point coords)
    {
        assert(coords.x < width&& coords.y < height);

        Row* row = GetRow(coords.y);
        row->size--;
        T* value = row[x];
        delete value;
        value = nullptr;

        return true;
    }

    bool Erase(uint index)
    {
        assert(index < width* height);
        Point coords = FromIndexToCoords(index);
        Row* row = GetRow(coords.y);

        row->size--;
        T* value = row[x];
        delete value;
        value = nullptr;

        return true;
    }

    // Returns the not empty cells sequentially
    // Optimized to iterate
    T* operator[](uint index)
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

        return T();
    }

private:
    
    Row* GetRow(uint y) const
    {
        return grid->At(y);
    }

    Point FromIndexToCoords(uint index)
    {
        uint y = index / width;
        uint x = index - width * y;

        return { x, y };
    }

private:

    Array<Row*>* grid = nullptr;

    uint width = 0;
    uint height = 0;
    uint size = 0;

};
