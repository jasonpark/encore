#ifndef ATOMIC_CVMX_H
#define ATOMIC_CVMX_H

#ifdef __cplusplus
extern "C" {
#endif

static inline sync(void)
{
  asm volatile("sync": : :"memory);
}

static inline void atomic_add32(int32_t *ptr, int32_t incr)
{
  sync();
  __asm__ __volatile__
  (
  "  saa %[inc], (%[base]) \n"
  : "+m" (*ptr)
  : [inc] "r" (incr), [base] "r" (ptr)
  : "memory"
  );
  sync(); 
}
 
static inline void atomic_set32(int32_t *ptr, int32_t value)
{
  sync();
  *ptr = value;
  sync();
}

static inline int32_t atomic_get32(int32_t *ptr)
{
  return *(volatile int32_t *)ptr;
}

static inline void atomic_add64(int64_t *ptr, int64_t incr)
{
  sync();
  __asm__ __volatile__ 
  ("   saad %[inc], (%[base])   \n"
  : "+m" (*ptr)
  : [inc] "r" (incr), [base] "r" (ptr)
  : "memory"
  );
  sync();
}

static inline void atomic_set64(int64_t *ptr, int64_t value)
{
  sync();
  *ptr = value;
  sync();
}

static inline int64_t atomic_get64(int64_t *ptr)
{
  return *(volatile int64_t *)ptr;
}

static inline uint64_t atomic_cas64(uint64_t *ptr, uint64_t old, uint64_t new)
{
   uint64_t ret;
   uint64_t tmp;
   sync();
   __asm__ __volatile__ 
   (".set noreorder           \n"
    "1: lld  %[tmp], %[val]   \n"
    "   li   %[ret], 0        \n"
    "   bne  %[tmp], %[old], 2f \n"
    "   move %[tmp], %[new]  \n"
    "   scd  %[tmp], %[val]  \n"
    "   beqz %[tmp], 1b      \n"
    "   li   %[ret], 1       \n"
    "2: nop                  \n"
    ".set reorder            \n"
    :  [val] "+m" (*ptr), [tmp] "=&r" (tmp), [ret] "=&r" (ret)
    :  [old] "r"  (old),  [new] "r" (new)
    : "memory"
    );
    sync();
    return (ret);
}
   
  
   return result;
}
  
   




#ifdef __cplusplus
}
#endif

#endif /* ATOMIC_CVMX_H */
