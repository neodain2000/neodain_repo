/*
** BASE
** 1. Little Edian
** __i386__ : Intel 32bit_
** __x86_64__ : Intel 64bit
** __arm__ : Arm
** 2. Big Edian
** __ppc__ : Motorola Power PC family
** __m68k__ : Motorola m68k/Coldfire
*/

#ifndef __NEO_OS_COMMON_TYPES_H__
#define __NEO_OS_COMMON_TYPES_H__

namespace neodain {
namespace neo {
namespace os {

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __NEO_OS_USE_ATTRIBUTE
#define __NEO_OS_USE_ATTRIBUTE
#ifdef __NEO_OS_USE_ATTRIBUTE__
#define _EXTENSION_         __extension__
#define NEO_OS_PACK         __attribute__ ((packed))
#define NEO_OS_ALIGN(n)     __attribute__((aligned(n))) // n byte align
#define NEO_OS_USED         __attribute__((used)) // static variable/function
#define NEO_OS_PRINTF(n,m)  __attribute__ ((format (printf, n, m)))
#endif
#endif

#if defined(_ix86_) || defined (__i386__)
/* ----------------------- Intel x86 processor family -------------------------*/
/* Little endian */
#undef _STRUCT_HIGH_BIT_FIRST_
#define _STRUCT_LOW_BIT_FIRST_

typedef unsigned char                           osalbool;
typedef signed char                             int8;
typedef short int                               int16;
typedef long int                                int32;
_EXTENSION_ typedef long long int               int64;
typedef unsigned char                           uint8;
typedef unsigned short int                      uint16;
typedef unsigned long int                       uint32;
_EXTENSION_ typedef unsigned long long int      uint64;
typedef unsigned long int                       cpuaddr;
typedef unsigned long int                       cpusize;
typedef long int                                cpudiff;

#elif defined (_ix64_) || defined (__x86_64__)
/* ----------------------- Intel/AMD x64 processor family -------------------------*/
/* Little endian */
#undef _STRUCT_HIGH_BIT_FIRST_
#define _STRUCT_LOW_BIT_FIRST_

typedef unsigned char                           osalbool;
typedef signed char                             int8;
typedef short int                               int16;
typedef int                                     int32;
typedef long int                                int64;
typedef unsigned char                           uint8;
typedef unsigned short int                      uint16;
typedef unsigned int                            uint32;
typedef unsigned long int                       uint64;
typedef unsigned long int                       cpuaddr;
typedef unsigned long int                       cpusize;
typedef long int                                cpudiff;

#elif defined (__ARM__) || defined(__arm__)
/* ----------------------- ARM processor family -------------------------*/
/* Little endian */
#undef   _STRUCT_HIGH_BIT_FIRST_
#define  _STRUCT_LOW_BIT_FIRST_

typedef unsigned char                           osalbool;
typedef signed char                             int8;
typedef short int                               int16;
typedef long int                                int32;
_EXTENSION_ typedef long long int               int64;
typedef unsigned char                           uint8;
typedef unsigned short int                      uint16;
typedef unsigned long int                       uint32;
_EXTENSION_ typedef unsigned long long int      uint64;
typedef unsigned long int                       cpuaddr;
typedef unsigned long int                       cpusize;
typedef long int                                cpudiff;

#elif defined(__PPC__) || defined (__ppc__) || defined(_m68k_) || defined(__m68k__) || defined(__SPARC__) || defined (_sparc_)
/* ----------------------- Motorola Power PC family ---------------------------*/
/* The PPC can be programmed to be big or little endian, we assume native */
/* Big endian */
#define _STRUCT_HIGH_BIT_FIRST_
#undef  _STRUCT_LOW_BIT_FIRST_

typedef unsigned char                           osalbool;
typedef signed char                             int8;
typedef short int                               int16;
typedef long int                                int32;
_EXTENSION_ typedef long long int               int64;
typedef unsigned char                           uint8;
typedef unsigned short int                      uint16;
typedef unsigned long int                       uint32;
_EXTENSION_ typedef unsigned long long int      uint64;
typedef unsigned long int                       cpuaddr;
typedef unsigned long int                       cpusize;
typedef long int                                cpudiff;

#else  /* not any of the above */
   #error undefined processor
#endif  /* processor types */


#ifndef NULL /* pointer to nothing */
#define NULL ((void *) 0)
#endif

#ifndef TRUE /* Boolean true */
#define TRUE (1)
#endif

#ifndef FALSE /* Boolean false */
#define FALSE (0)
#endif


/*
** Condition = TRUE is ok, Condition = FALSE is error
*/
#define CompileTimeAssert(Condition, Message) typedef char Message[(Condition) ? 1 : -1]

/*
** Check Sizes
*/
CompileTimeAssert(sizeof(uint8)==1, TypeUint8WrongSize);
CompileTimeAssert(sizeof(uint16)==2, TypeUint16WrongSize);
CompileTimeAssert(sizeof(uint32)==4, TypeUint32WrongSize);
CompileTimeAssert(sizeof(uint64)==8, TypeUint64WrongSize);
CompileTimeAssert(sizeof(int8)==1, Typeint8WrongSize);
CompileTimeAssert(sizeof(int16)==2, Typeint16WrongSize);
CompileTimeAssert(sizeof(int32)==4, Typeint32WrongSize);
CompileTimeAssert(sizeof(int64)==8, Typeint64WrongSize);
CompileTimeAssert(sizeof(cpuaddr) >= sizeof(void *), TypePtrWrongSize);

#if !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER)

#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || \
    defined(__BIG_ENDIAN__) || \
    defined(__ARMEB__) || \
    defined(__THUMBEB__) || \
    defined(__AARCH64EB__) || \
    defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
/* It is a big-endian target architecture */
#define SOFTWARE_BIG_BIT_ORDER
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || \
    defined(__LITTLE_ENDIAN__) || \
    defined(__ARMEL__) || \
    defined(__THUMBEL__) || \
    defined(__AARCH64EL__) || \
    defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
/* It is a little-endian target architecture */
#define SOFTWARE_LITTLE_BIT_ORDER
#endif

#endif /* !defined(SOFTWARE_BIG_BIT_ORDER) && !defined(SOFTWARE_LITTLE_BIT_ORDER) */

#ifdef __cplusplus
}
#endif



} // namespace os
} // namespace neo
} // namespace neodain


#endif // __NEO_OS_COMMON_TYPES_H__


