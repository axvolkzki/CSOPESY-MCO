#pragma once
#include "../Memory/IMemoryAllocator.h"
#include "../TypedefRepo.h"
#include <unordered_map>

class FlatMemoryAllocator : public IMemoryAllocator {
public:
    FlatMemoryAllocator(size_t maximumSize);
    ~FlatMemoryAllocator();

    void* allocate(size_t) override;
    void deallocate(void* ptr) override;
    String visualizeMemory() override;

private:
    size_t maximumSize;
    size_t allocatedSize;
    std::vector<char> memory;
    std::unordered_map<size_t, bool> allocationMap;

    void initializeMemory();
    bool canAllocateAt(size_t index, size_t size) const;
    void allocateAt(size_t index, size_t size);
    void deallocateAt(size_t index);
};
