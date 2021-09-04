#ifndef _TYPE_H_
#define _TYPE_H_

typedef void sl_void;
typedef unsigned char sl_u8;
typedef char sl_s8;
typedef unsigned short sl_u16;
typedef short sl_s16;
typedef unsigned int sl_u32;
typedef int sl_s32;

#define sl_ok 0
#define sl_error !sl_ok

#ifndef NULL
#define NULL (sl_void *)0
#endif

#endif
