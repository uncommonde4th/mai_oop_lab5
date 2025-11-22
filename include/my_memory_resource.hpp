#pragma once

#include <vector>
#include <algorithm>
#include <stdexcept>
#include <memory_resource>

class MyMemoryResource : public std::pmr::memory_resource { 
private:

    struct memBlock {
        size_t size;
        size_t offset;
        memBlock(size_t o = 0, size_t s = 0) : offset(o), size(s) {}
    };

    static constexpr size_t BUFF_SIZE{1024};
    char mem_buff[BUFF_SIZE];
    std::vector<memBlock> used_blocks;

public:

    MyMemoryResource() = default;

protected:

    void *do_allocate(size_t bytes, size_t alignment) override {
        size_t alloc_offset{0};

        std::sort(used_blocks.begin(), used_blocks.end(), [](const memBlock &l, const memBlock &r) { return l.offset < r.offset; });

        for (const memBlock &block : used_blocks) {
            if (alloc_offset + bytes <= block.offset) { break; }
            else { alloc_offset = block.offset + block.size; }
        }
        
        if (alloc_offset + bytes > BUFF_SIZE) {
            throw std::bad_alloc();
        }

        used_blocks.emplace_back(alloc_offset, bytes);

        return mem_buff + alloc_offset;
    }

    void do_deallocate(void *ptr, size_t bytes, size_t alignment) override {
        char *block = static_cast<char *>(ptr);

        for (std::vector<memBlock>::iterator i = used_blocks.begin(); i != used_blocks.end(); ++i) {
            if (block == mem_buff + i->offset) {
                used_blocks.erase(i);
                return;
            }
        }

        throw std::logic_error("Попытка освободить не выделенный блок.");
    }

    bool do_is_equal(const std::pmr::memory_resource &other) const noexcept override {
        return this == &other;
    }
};
