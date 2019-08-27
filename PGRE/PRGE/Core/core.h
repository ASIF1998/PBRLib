#ifndef CORE_H
#define CORE_H

/**
 * Макрос определяющий, необходимо ли включать код для отладки или нет.
*/
#define DEBUG_PRGE 1

#if DEBUG_PRGE == 1
#   define NOEXCEPT_PRGE
#else 
#   define NOEXCEPT_PRGE noexcept
#endif

#endif // CORE_H
