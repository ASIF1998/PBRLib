//
//  memoryArena.inl
//  PRGE
//
//  Created by Асиф Мамедов on 25/08/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../core.h"

#include <cstdint>

#include <list>
#include <map>

#include <algorithm>

using namespace std;

namespace PRGE
{
    class MemoryArena
    {
        friend class STLMemoryArena;
        friend class MemoryBlock;

    public:

        /**
         * Конструктор.
         * 
         * @param blockSize размер выделяемого блока(по умолчанию 256 кб)
        */
        inline MemoryArena(size_t blockSize = 262144) :
            _bolckSize{blockSize},
            _currentBlockPos{0},
            _currentBlockSize{0},
            _ptrBlock{nullptr}
        {
#if DEBUG_PRGE == 1
            if (!blockSize) {
                throw invalid_argument ("blockSize is zero");
            }
#endif
        }

        inline MemoryArena(const MemoryArena& memoryArena) :
            _bolckSize{memoryArena._bolckSize},
            _currentBlockPos{0},
            _currentBlockSize{0},
            _ptrBlock{nullptr},
            _usedBlock(memoryArena._usedBlock.size())
        {}

        inline MemoryArena(MemoryArena&& memoryArena) :
            _bolckSize{memoryArena._bolckSize},
            _currentBlockPos{0},
            _currentBlockSize{0},
            _ptrBlock{memoryArena._ptrBlock}
        {
            swap(_usedBlock, memoryArena._usedBlock);
            swap(_availableBlock, memoryArena._availableBlock);
        }

        MemoryArena& operator = (const MemoryArena&) = delete;
        MemoryArena& operator = (MemoryArena&&) = delete;

        /**
         * Метод необходимый для выделения памяти.
         * 
         * @param bytes количество требуемой памяти
         * @return указатель на начало памяти
        */
        uint8_t* allocator(size_t bytes)
        {
            /// Выраниваем количество байт до размера кратного 16.
            bytes = (bytes + 15) & (~ 15);

            if (_currentBlockPos + bytes > _currentBlockSize) {
                if (_ptrBlock) {
                    _usedBlock.push_back(make_pair(_currentBlockSize, _ptrBlock));
                    _ptrBlock = nullptr;
                }

                if (auto ptrNewBlock {_availableBlock.lower_bound(bytes)}; ptrNewBlock != _availableBlock.end()) {
                    _ptrBlock = ptrNewBlock->second;
                    _currentBlockSize = ptrNewBlock->first;
                    _availableBlock.erase(ptrNewBlock);
                }

                if (!_ptrBlock) {
                    _currentBlockSize = max(_bolckSize, bytes);
                    _ptrBlock = MemoryArena::_allocator(_currentBlockSize);
                }

                _currentBlockPos = 0;
            }

            auto* outPtr = _ptrBlock + _currentBlockPos;
            _currentBlockPos += bytes;

            return outPtr;
        }

        /**
         * Метод необходимый для выделения памяти.
         * 
         * @param size количество выделенных объектов для типа Type
         * @param runConstructor true - вызвать конструктор по умолчанию, false - не вызывать конструктор по умолчанию
        */
        template<typename Type>
        Type* allocator(size_t size, bool runConstructor = true) {
            Type* ptr = reinterpret_cast<Type*>(allocator(sizeof(Type) * size));

            if (runConstructor) {
                for (size_t i{0}; i < size; i++) {
                    new (ptr + i) Type();
                }
            }

            return ptr;
        }

        void reset()
        {
            _currentBlockPos = 0;
            _availableBlock.insert(make_move_iterator(_usedBlock.begin()), make_move_iterator(_usedBlock.end()));
        }

        /**
         * Метод, возвращающий количесвто всей выдеоенной памяти.
         * 
         * @return количесвто всей выдеоенной памяти
        */
        size_t totalAlloca() const
        {
            size_t size = 0;

            for (auto&& ptrUsedBlock: _usedBlock) {
                size += ptrUsedBlock.first;
            }

            for (auto&& ptrAvailableBlock: _availableBlock) {
                size += ptrAvailableBlock.first;
            }

            return size;
        }

    private:
        static uint8_t* _allocator(size_t bytes)
        {
#if DEBUG_PRGE == 1
            if (!bytes) {
                throw invalid_argument ("bytes is zero");
            }
#endif

#if (defined (_WIN32) || defined (_WIN64))
            return _aligned_malloc(bytes, PRGE_MEMORY_ALLOCATING_CASH_LINE_SIZE);
#else
            void* ptr;

                if (posix_memalign(&ptr, PRGE_MEMORY_ALLOCATING_CASH_LINE_SIZE, bytes)) {
                    ptr = nullptr;
                }

                return reinterpret_cast<uint8_t*>(ptr);
#endif
        }

    private:
        size_t _bolckSize;
        size_t _currentBlockPos;
        size_t _currentBlockSize;
        uint8_t* _ptrBlock;
        list<pair<size_t, uint8_t*>> _usedBlock;
        multimap<size_t, uint8_t*> _availableBlock;
    };
}
