#ifndef _LIMIT_H_
#define _LIMIT_H_

typedef struct LimitAttrTag
{
    sl_u32 tolerance;
    sl_u32 eleCnt;
} LimitAttr;

extern sl_s32 filter_limit_global_init(sl_u8 channelCnt);
extern sl_s32 filter_limit_attr_init(sl_u8 channel, LimitAttr *pAttr);
extern sl_u32 filter_limit(sl_u8 channel, sl_u32 val);
extern sl_void filter_limit_exit(sl_void);

#endif
