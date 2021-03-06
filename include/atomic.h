#ifndef  ATOMIC_H
#define ATOMIC_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* atomic operation 
   sequentially consistent : fetch_and_store, fetch_and_add, compare_and_swap(cas) 
 */


#if defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__)) 


static inline uint32_t atomic_add32 ( volatile uint32_t *ptr, uint32_t value)
{
  uint32_t result;
  
  asm volatile
  ("lock; xadd %1, %0"
  : "+m"(*ptr), "=r"(result)
  : "1"(value)
  : "memory", "cc"
  );
    
  return result;
}

inline uint32_t atomic_inc32(volatile uint32_t *ptr)
{
  return atomic_add32(ptr, 1);
}

inline uint32_t atomic_dec32(volatile uint32_t *ptr)
{
  return atomic_add32(ptr, (uint32_t)(-1));
}

static inline uint32_t atomic_fetch_and_add32 (volatile uint32_t *ptr, uint32_t value)
{
  uint32_t result;

  asm volatile
  ("lock; xadd %0, %1"
  : "=r"(result), "=m" (*(ptr))
  : "0"(value), "m"(*(ptr))
  : "memory", "cc"
  );

 return result;     
}

static inline uint32_t atomic_cas32 (volatile uint32_t *ptr, uint32_t expectedValue, uint32_t newValue)
{
  uint32_t prev = newValue;

  asm volatile
  ("lock; cmpxchg %3, %1"
  : "=a" (prev), "=m" (*(ptr))
  : "0"  (prev), "r"  (expectedValue)
  : "memory", "cc"
  );

  return prev;                  
}

static inline uint64_t atomic_cas64(volatile uint64_t *ptr, uint64_t expected, uint64_t newValue)
{
  uint64_t prev = newValue;

  asm volatile
  ("lock; cmpxchg q %3, %1"
  : "=a" (prev), "=m" (*(ptr))
  : "0" (prev), "r" (expected)
  : "memory", "cc"
  );

  return prev;                  
}

#endif 


#ifdef __cplusplus
} 
#endif 

#endif  
