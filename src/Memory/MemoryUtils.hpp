//
//  MemoryUtils.hpp
//  PBRLibTests
//
//  Created by Асиф Мамедов on 26/04/2020.
//  Copyright © 2020 Асиф Мамедов. All rights reserved.
//

#ifndef MemoryUtils_hpp
#define MemoryUtils_hpp

#include <cstddef>

/**
 * Если константа препроцессора PBRLIB_L1_CACHE_LINE_SIZE не установлена,
 * то размер кеш строки, по умолчанию, становится равной 64 байтам.
*/
#ifndef PBRLIB_L1_CACHE_LINE_SIZE
#   define PBRLIB_L1_CACHE_LINE_SIZE 64
#endif

namespace pbrlib
{
    /**
     * @brief Функция, необходимая для выделения выровненной памяти.
     * 
     * @param size размер требуемой памяти.
     * @return Указатель на выделенную память.
    */
    void* allocAligned(size_t size);

    /**
     * @brief 
     *      Функция, необходимая для освобождения памяти, которая была 
     *      выделена с помощью функции allocAligned(...).
     * 
     * @param ptr указатель на освобождаемую память.
    */
    void freeAligned(void* ptr);

    size_t getAlignSize(size_t size);
}

#endif /* MemoryUtils_hpp */
