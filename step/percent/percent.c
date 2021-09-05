#include "..\..\common\type.h"
#include "percent.h"
#include <stdlib.h>
#include <memory.h>

struct {
    sl_u8 channelCnt;
    sl_u32 *pbackVal;
    PercentAttr *pattr;
} PercentGlobal;

sl_s32 step_percent_global_init(sl_u8 channelCnt)
{
    PercentGlobal.channelCnt = channelCnt;

    PercentGlobal.pbackVal = malloc(sizeof(sl_u32) * channelCnt);
    if (!PercentGlobal.pbackVal) {
        return sl_error;
    }
    memset(PercentGlobal.pbackVal, 0, sizeof(sl_u32) * channelCnt);

    PercentGlobal.pattr = malloc(sizeof(PercentAttr) * channelCnt);
    if (!PercentGlobal.pattr) {
        return sl_error;
    }
    memset(PercentGlobal.pattr, 0, sizeof(PercentAttr) * channelCnt);

    return sl_ok;
}

sl_s32 step_percent_attr_init(sl_u8 channel, PercentAttr *pAttr)
{
    if (channel > PercentGlobal.channelCnt - 1) {
        return sl_error;
    }

    PercentGlobal.pattr[channel] = *pAttr;

    return sl_ok;
}

sl_u32 step_percent_add(sl_u8 channel, sl_u32 curVal)
{
    sl_u32 difference = curVal - PercentGlobal.pbackVal[channel];
    sl_u32 increment = difference * PercentGlobal.pattr[channel].percentage / 100;

    if (!increment) {
        increment = PercentGlobal.pattr[channel].leastStep;
    }

    return PercentGlobal.pbackVal[channel] + increment;
}

sl_u32 step_percent_subtract(sl_u8 channel, sl_u32 curVal)
{
    sl_u32 difference = PercentGlobal.pbackVal[channel] - curVal;
    sl_u32 decrement = difference * PercentGlobal.pattr[channel].percentage / 100;

    if (!decrement) {
        decrement = PercentGlobal.pattr[channel].leastStep;
    }

    return PercentGlobal.pbackVal[channel] - decrement;
}

sl_u32 step_percent_handle(sl_u8 channel, sl_u32 curVal)
{
    if (channel > PercentGlobal.channelCnt - 1) {
        return 0;
    }

    if (curVal > PercentGlobal.pbackVal[channel]) {
        PercentGlobal.pbackVal[channel] = step_percent_add(channel, curVal);
    } else if (curVal < PercentGlobal.pbackVal[channel]) {
        PercentGlobal.pbackVal[channel] = step_percent_subtract(channel, curVal);
    }

    return PercentGlobal.pbackVal[channel];
}

sl_void step_percent_exit(sl_void)
{
    sl_u8 channelIdx;

    for (channelIdx = 0; channelIdx < PercentGlobal.channelCnt; channelIdx++) {
        if (PercentGlobal.pattr) {
            free(PercentGlobal.pattr);
            PercentGlobal.pattr = NULL;
        }

        if (PercentGlobal.pbackVal) {
            free(PercentGlobal.pbackVal);
            PercentGlobal.pbackVal = NULL;
        }
    }

    return;
}
