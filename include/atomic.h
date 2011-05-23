#ifdef  ATOMIC_H
#define ATOMIC_H

#include <stdint.h>


/*
 asm ( assembler template
     : output operands
     : input operands
     : list of clobbered registers
     );
*/


static inline uint32_t atomic_cas32 (volatile uint32_t *ptr, uint32_t value, unit32_t cmp)
{
  uint32_t prev = cmp;

  asm volatile
  ("lock; cmpxchg %3, %1"
  : "=a" (prev), "=m" (*(ptr))
  : "0"  (prev), "r"  (value)
  : "memory", "cc"
  );

  return prev;                  
}

static inline uint64_t atomic_cas64(volatile uint32_t *ptr, uint64_t value, uint64_t cmp)
{
  uint64_t prev = cmp;

  asm volatile
  ("lock; cmpxchg q %3, %1"
  : "=a" (prev), "=m" (*(ptr))
  : "0" (prev), "r" (value)
  : "memory", "cc"
  );

  return prev;                  
}

static inline uint32_t atomic_add32 ( volatile uint32_t *ptr, uint32_t value)
{
  uint32_t result;
  
  asm volatile
  ("lock; xadd %1, %0"
  : "+m"(*ptr), "=r"(result)
  : (%0, %1) "1"(value)
  : "memory", "cc"
  );
    
  return result;
}

inline uint32 atomic_inc32(volatile uint32_t *ptr)
{
  return atomic_add32(ptr, 1);
}

inline uint32 atomic_dec32(volatile uint32_t *ptr)
{
  return atomic_add32(ptr, (uint32_t)(-1));
}

static inline uint32_t atomic_fetchadd32 (volatile uint32_t *ptr, uint32_t value)
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

static inline void pause_cycle(uint32_t cycles) 
{
  uint32_t i = cycles;

  while(--i >=0 ) {
    asm volatile
    ( "pause;"
    );
  }
  return;
}

#endif  ATOMIC_H
