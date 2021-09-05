#ifndef _PERCENT_H_
#define _PERCENT_H_

typedef struct PercentAttrTag {
    sl_u8 percentage;
    sl_u32 leastStep;
} PercentAttr;

extern sl_s32 step_percent_global_init(sl_u8 channelCnt);
extern sl_s32 step_percent_attr_init(sl_u8 channel, PercentAttr *pAttr);
extern sl_u32 step_percent_handle(sl_u8 channel, sl_u32 curVal);
extern sl_void step_percent_exit(sl_void);

#endif