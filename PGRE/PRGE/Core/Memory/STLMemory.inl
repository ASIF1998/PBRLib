//
//  STLMemory.inl
//  PRGE
//
//  Created by Асиф Мамедов on 25/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <cstdint>

#include <map>
#include <unordered_map>
#include <unordered_set>

#include <atomic>

#include <memory>

#include "memoryArena.hpp"

#include <iostream>

using namespace std;

namespace PRGE
{
    struct MemoryBlock
    {
        inline MemoryBlock(size_t memoryBlockSize) :
            ptrMemoryBlock{MemoryArena::_allocator(memoryBlockSize)},
            currentMemoryBlockSize{memoryBlockSize},
            currentPositioninMemoryBlock{0},
            count{0}
        {}

        inline MemoryBlock(const MemoryBlock& memoryBlock) :
            ptrMemoryBlock{MemoryArena::_allocator(memoryBlock.currentMemoryBlockSize)},
            currentMemoryBlockSize{memoryBlock.currentMemoryBlockSize},
            currentPositioninMemoryBlock{0},
            count{0}
        {}

        inline ~MemoryBlock()
        {
            cerr << "~MemoryBlock" << endl;
            delete [] ptrMemoryBlock;
        }

        uint8_t* ptrMemoryBlock;
        size_t currentMemoryBlockSize;
        size_t currentPositioninMemoryBlock;
        uint64_t count;

    };

    struct PartMemoryBlock
    {
        // partMemory: _ptrCurrentBlock, _ptrCurrentBlock->ptrMemoryBlock + _ptrCurrentBlock->currentPositioninMemoryBlock, bytes
        PartMemoryBlock(MemoryBlock* ptrMemoryBlock, uint8_t* ptrPartMemoryBlock, size_t size) :
            ptrMemoryBlock{ptrMemoryBlock},
            ptrPartMemoryBlock{ptrPartMemoryBlock},
            size{size}
        {}
        
        PartMemoryBlock(const PartMemoryBlock& partMemoryBlock) :
            ptrMemoryBlock{partMemoryBlock.ptrMemoryBlock},
            ptrPartMemoryBlock{partMemoryBlock.ptrPartMemoryBlock},
            size{partMemoryBlock.size}
        {}
        
        PartMemoryBlock(PartMemoryBlock&& partMemoryBlock)
        {
            swap(ptrMemoryBlock, partMemoryBlock.ptrMemoryBlock);
            swap(ptrPartMemoryBlock, partMemoryBlock.ptrPartMemoryBlock);
            swap(size, partMemoryBlock.size);
        }
        
        ~PartMemoryBlock()
        {
            cerr << "~PartMemoryBlock" << endl;
        }
        
        MemoryBlock* ptrMemoryBlock;
        uint8_t* ptrPartMemoryBlock;
        size_t size;
    };

    class STLMemoryArena
    {
    public:
        /**
         * Конструктор.
         * 
         * @param memoryBlockSize размер выделяемого блока(по умолчанию 256 кб)
        */
        inline STLMemoryArena(size_t memoryBlockSize = PRGE_MEMORY_BLOCK_SIZE) :
            _ptrCurrentBlock{new MemoryBlock (memoryBlockSize)},
            _memoryBlockSize{memoryBlockSize}
        {}

        inline STLMemoryArena(const STLMemoryArena& memoryArena) :
            _usedPartMemoryBlock{memoryArena._usedPartMemoryBlock.size()},
            _usedBlocks{memoryArena._usedBlocks.size()},
            _ptrCurrentBlock{new MemoryBlock (*memoryArena._ptrCurrentBlock)},
            _memoryBlockSize{memoryArena._memoryBlockSize}
        {}

        inline STLMemoryArena(STLMemoryArena&& memoryArena) :
            _memoryBlockSize{memoryArena._memoryBlockSize}
        {
            swap(_ptrCurrentBlock, memoryArena._ptrCurrentBlock);
            swap(_availableBlocks, memoryArena._availableBlocks);
            swap(_usedPartMemoryBlock, memoryArena._usedPartMemoryBlock);
            swap(_usedBlocks, memoryArena._usedBlocks);
        }

        inline ~STLMemoryArena()
        {
            cerr << "~STLMemoryArena" << endl;
            delete _ptrCurrentBlock;
        }

        // STLMemoryArena& operator = (const STLMemoryArena&) = delete;
        // STLMemoryArena& operator = (STLMemoryArena&&) = delete;

        STLMemoryArena& operator = (const STLMemoryArena&)
        {
            return *this;
        }

        STLMemoryArena& operator = (STLMemoryArena&&)
        {
            return *this;
        }

        uint8_t* allocate(size_t bytes)
        {
            static int rc = 0;
            
            cout << "Inseart RC: " << rc << endl;
            rc++;
            
            /// Выраниваем количество байт до размера кратного 16.
            bytes = (bytes + 15) & (~ 15);

            if (_ptrCurrentBlock->currentPositioninMemoryBlock + bytes > _ptrCurrentBlock->currentMemoryBlockSize) {
                _usedBlocks.insert(_ptrCurrentBlock);
                _ptrCurrentBlock = nullptr;

                if (auto ptr = _availableBlocks.lower_bound(bytes); ptr != _availableBlocks.end()) {
                    _ptrCurrentBlock = ptr->second;
                    _ptrCurrentBlock->currentPositioninMemoryBlock = 0;
                    _availableBlocks.erase(ptr);
                }

                if (!_ptrCurrentBlock) {
                    size_t size = max(_memoryBlockSize, bytes);
                    _ptrCurrentBlock = reinterpret_cast<MemoryBlock*>(MemoryArena::_allocator(size));
                    new (_ptrCurrentBlock) MemoryBlock (size);
                }
            }

            auto* partMemory = reinterpret_cast<PartMemoryBlock*>(MemoryArena::_allocator(sizeof(PartMemoryBlock)));
            
            new (partMemory) PartMemoryBlock (_ptrCurrentBlock, _ptrCurrentBlock->ptrMemoryBlock + _ptrCurrentBlock->currentPositioninMemoryBlock, bytes);
        
            /// TODO: разобраться с багом, в котром в _usedPartMemoryBlock попадают указатели типа NULL

            _ptrCurrentBlock->count++;
            _ptrCurrentBlock->currentPositioninMemoryBlock += bytes;

            _usedPartMemoryBlock.insert(make_pair(partMemory->ptrPartMemoryBlock, partMemory));
            
            for (auto&& ittter: _usedPartMemoryBlock) {
                if (!ittter.second->ptrPartMemoryBlock) {
                    
                    cout << "Zerror" << endl;
                }
            }

            return partMemory->ptrPartMemoryBlock;
        }

        template<typename Type>
        Type* allocate(size_t size, bool runConstructor = true)
        {
            auto* ptr = reinterpret_cast<Type*>(allocate(sizeof(Type) * size));

            if (runConstructor) {
                for (size_t i{0}; i < size; i++) {
                    new (ptr + i) Type();
                }
            }

            return ptr;
        }

        void deallocator(uint8_t* ptr)
        {
            auto fPtr = _usedPartMemoryBlock[ptr];

            if (!fPtr->ptrMemoryBlock->count) {
                auto ptrUsedBlock = _usedBlocks.find(fPtr->ptrMemoryBlock);
                
                if (ptrUsedBlock != _usedBlocks.end()) {
                    _usedBlocks.erase(_usedBlocks.find(fPtr->ptrMemoryBlock));
                    _availableBlocks.insert(make_pair(fPtr->ptrMemoryBlock->currentMemoryBlockSize, fPtr->ptrMemoryBlock));
                }
            }

            fPtr->ptrMemoryBlock->count--;
            _usedPartMemoryBlock.erase(_usedPartMemoryBlock.find(ptr));
        }

        template<typename Type>
        void deallocator(Type* ptr)
        {
            static int rc = 0;
            
            cout << "Delete RC: " << rc << endl;
            rc++;
            
            auto fIter = _usedPartMemoryBlock.find(reinterpret_cast<uint8_t*>(ptr));
            
            if (fIter != _usedPartMemoryBlock.end()) {

                fIter->second->ptrMemoryBlock->count--;

                if (!fIter->second->ptrMemoryBlock->count) {
                    auto ptrUsedBlock = _usedBlocks.find(fIter->second->ptrMemoryBlock);

                    if (ptrUsedBlock != _usedBlocks.end()) {
                        _usedBlocks.erase(_usedBlocks.find(fIter->second->ptrMemoryBlock));
                        _availableBlocks.insert(make_pair(fIter->second->ptrMemoryBlock->currentMemoryBlockSize, fIter->second->ptrMemoryBlock));
                    }
                }
            }
        }

        size_t capacity() const
        {
            size_t n = 0;
            for (auto&& iter: _availableBlocks) {
                n += iter.first;
            }

            for (auto&& iter: _usedBlocks) {
                n += iter->currentMemoryBlockSize;
            }

            return n;
        }

    private:
        multimap<size_t, MemoryBlock*> _availableBlocks;
        unordered_map<uint8_t*, PartMemoryBlock*> _usedPartMemoryBlock;
        unordered_set<MemoryBlock*> _usedBlocks;
        MemoryBlock* _ptrCurrentBlock;
        size_t _memoryBlockSize;
    };

    template<typename Type>
    class STLAllocator
    {
    public:
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using pointer = Type*;
        using const_pointer = const Type*;
        using reference = Type&;
        using const_reference = const Type&;
        using value_type = Type;

    //     typedef size_t    size_type;
	// typedef ptrdiff_t difference_type;
	// typedef T*        pointer;
	// typedef const T*  const_pointer;
	// typedef T&        reference;
	// typedef const T&  const_reference;
	// typedef T         value_type;
	// template<class U> struct rebind

        template<class U>
        struct redind
        {
            typedef STLAllocator<U> other;
        };

        STLAllocator(size_t memoryBlockSize = PRGE_MEMORY_BLOCK_SIZE) noexcept :
            ptrMemoryArena{new STLMemoryArena (memoryBlockSize)}
        {}

        STLAllocator(const STLAllocator& alloc) noexcept
        {}

        STLAllocator(STLAllocator&& alloc) noexcept
        {}

        template<typename U>
        STLAllocator(const STLAllocator<U>& alloc) noexcept
        {}

        ~STLAllocator() noexcept
        {
            delete ptrMemoryArena;
        }

        pointer allocate(size_type size)
        {
            return reinterpret_cast<Type*>((*ptrMemoryArena).allocate(size));
        }

        pointer allocate(size_type size, const void* outPtr)
        {
            return reinterpret_cast<Type*>((*ptrMemoryArena).allocate(size));
        }

        void deallocate(pointer p, size_type n)
        {
            (*ptrMemoryArena).deallocator(p);
        }


        pointer address(reference x) const
        {
            return &x;
        }

        const_pointer address(const_reference x) const
        {
            return &x;
        }

        size_type max_size() const
        {
            return numeric_limits<int>::max();
        }

        size_type capacity() const
        {
            return (*ptrMemoryArena).capacity();
        }

        template<class U>
        void destroy(U* p)
        {
            p->~U();
        }

//        bool operator == (const STLAllocator&) const noexcept
//        {
//            return true;
//        }
//
//        bool operator != (const STLAllocator&) const noexcept
//        {
//            return false;
//        }
        
        STLMemoryArena* ptrMemoryArena;
    };
}
