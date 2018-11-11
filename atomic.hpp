#ifndef VI_ATOMIC_HPP
#define VI_ATOMIC_HPP

//these macro taken from https://locklessinc.com/articles/locks/

#define atomic_add(P, V) __sync_add_and_fetch((P), (V))

#define atomic_xadd(P, V) __sync_fetch_and_add((P), (V))

#define atomic_read(V) (*(volatile typeof(V) *)&(V))

#define barrier() asm volatile("": : :"memory")

#define cpu_relax() asm volatile("pause\n": : :"memory")

static inline unsigned xchg_32(void *ptr, unsigned x)
{
	__asm__ __volatile__("xchgl %0,%1"
				:"=r" ((unsigned) x)
				:"m" (*(volatile unsigned *)ptr), "0" (x)
				:"memory");

	return x;
}

static inline unsigned long long xchg_64(void *ptr, unsigned long long x)
{
	__asm__ __volatile__("xchgq %0,%1"
				:"=r" ((unsigned long long) x)
				:"m" (*(volatile unsigned long long *)ptr), "0" (x)
				:"memory");

	return x;
}

#endif