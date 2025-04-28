#ifndef _MACRO_H
#define _MACRO_H

#define UINT_MAX                0xffffffff

#define BIT(n)                  (1ul << (n))

#define AT(x)                   __attribute__((section(#x)))
#define ALIGNED(n)              __attribute__((aligned(n)))
#define DMA_ADR(x)              ((u32)x)
#define ALWAYS_INLINE           __attribute__((always_inline)) inline
#define NO_INLINE               __attribute__((noinline))
#define WEAK                    __attribute__((weak))
#define PACKED                  __attribute__((packed))
#define FIQ                     __attribute__((fiq("machine")))

#define WDT_CLR()               WDTCON = 0xa
#define WDT_EN()                WDTCON = 0x110
#define WDT_DIS()               WDTCON = 0xaa0
#define WDT_RST()               WDTCON = 0xa000110; while (1)
#define WDT_RST_DELAY()         WDTCON = 0xa100110; while (1)

#define BYTE0(n)                ((unsigned char)(n))
#define BYTE1(n)                ((unsigned char)((n)>>8))
#define BYTE2(n)                ((unsigned char)((n)>>16))
#define BYTE3(n)                ((unsigned char)((n)>>24))

#define GET_LE16(ptr)           (u16)(*(u16*)(u8*)(ptr))
#define GET_LE32(ptr)           (u32)(*(u32*)(u8*)(ptr))
#define PUT_LE16(ptr, val)      *(u16*)(u8*)(ptr) = (u16)(val)
#define PUT_LE32(ptr, val)      *(u32*)(u8*)(ptr) = (u32)(val)

#define GET_BE16(ptr)           get_be16(ptr)
#define GET_BE32(ptr)           get_be32(ptr)
#define PUT_BE16(ptr, val)      put_be16(ptr, val)
#define PUT_BE32(ptr, val)      put_be32(ptr, val)

#define GLOBAL_INT_DISABLE()            uint32_t cpu_ie = PICCON&BIT(0); PICCONCLR = BIT(0); asm volatile("":::"memory")
#define GLOBAL_INT_RESTORE()            asm volatile("":::"memory"); PICCON |= cpu_ie

#define CONST_CAT(x, y)         x ## y
#define SET_MACRO(x, y)         CONST_CAT(x, y)

///编译阶段检测结构体的大小是否等于特定值
#define SIZE_OF_TYPE_EQUAL_TO(type, size) \
static inline char size_of_##type##_equal_to_##size() \
{ \
    char __dummy1[sizeof(type) - size]; \
    char __dummy2[size - sizeof(type)]; \
    return __dummy1[0] + __dummy2[0];  \
}

///编译阶段检测结构体成员在结构体的偏移是否等于特定值
#define OFFSET_OF_TYPE_EQUAL_TO(type, member, size) \
static inline char OFFSET_of_##member##_in_##type##_equal_to_##size() \
{ \
    char __dummy1[offsetof(type,member) - size]; \
    char __dummy2[size - offsetof(type,member)]; \
    return __dummy1[0] + __dummy2[0]; \
}

#endif // _MACRO_H
