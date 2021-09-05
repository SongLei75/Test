#include "..\..\common\type.h"
#include "limit.h"
#include <stdlib.h>
#include <memory.h>

struct {
    sl_u8 channelCnt;
    LimitAttr *pattr;
    sl_u32 **pbuffer;
    sl_u32 *peleIdx;
    sl_u32 *pretVal;
} LimitGlobal;

sl_s32 filter_limit_global_init(sl_u8 channelCnt)
{
    memset(&LimitGlobal, 0, sizeof(LimitGlobal));

    LimitGlobal.channelCnt = channelCnt;
    if (LimitGlobal.channelCnt == 0) {
        return sl_ok;
    }

    LimitGlobal.pattr = malloc(sizeof(LimitAttr) * LimitGlobal.channelCnt);
    if (!LimitGlobal.pattr) {
        return sl_error;
    }
    memset(LimitGlobal.pattr, 0, sizeof(LimitAttr) * LimitGlobal.channelCnt);

    LimitGlobal.pbuffer = malloc(sizeof(sl_u32 **) * LimitGlobal.channelCnt);
    if (!LimitGlobal.pbuffer) {
        return sl_error;
    }
    memset(LimitGlobal.pbuffer, 0, sizeof(sl_u32 **) * LimitGlobal.channelCnt);

    LimitGlobal.peleIdx = malloc(sizeof(sl_u32) * LimitGlobal.channelCnt);
    if (!LimitGlobal.peleIdx) {
        return sl_error;
    }
    memset(LimitGlobal.peleIdx, 0, sizeof(sl_u32) * LimitGlobal.channelCnt);

    LimitGlobal.pretVal = malloc(sizeof(sl_u32) * LimitGlobal.channelCnt);
    if (!LimitGlobal.pretVal) {
        return sl_error;
    }
    memset(LimitGlobal.pretVal, 0, sizeof(sl_u32) * LimitGlobal.channelCnt);

    return sl_ok;
}

sl_s32 filter_limit_attr_init(sl_u8 channel, LimitAttr *pAttr)
{
    if (channel > LimitGlobal.channelCnt - 1) {
        return sl_error;
    }

    if (pAttr->eleCnt == 0) {
        return sl_error;
    }

    LimitGlobal.pattr[channel] = *pAttr;

    LimitGlobal.pbuffer[channel] = malloc(sizeof(sl_u32) * pAttr->eleCnt);
    if (!LimitGlobal.pbuffer[channel]) {
        return sl_error;
    }
    memset(LimitGlobal.pbuffer[channel], 0, sizeof(sl_u32) * pAttr->eleCnt);

    return sl_ok;
}

sl_u32 filter_limit_average(sl_u8 channel)
{
    sl_u32 eleLoop;
    sl_u32 total = 0;

    for (eleLoop = 0; eleLoop < LimitGlobal.pattr[channel].eleCnt; eleLoop++) {
        total += LimitGlobal.pbuffer[channel][eleLoop];
    }

    return (total / LimitGlobal.pattr[channel].eleCnt);
}

sl_u32 filter_limit(sl_u8 channel, sl_u32 val)
{
    if (channel > LimitGlobal.channelCnt - 1) {
        return 0;
    }

    if (val >= LimitGlobal.pretVal[channel]) {
        if (val - LimitGlobal.pretVal[channel] > LimitGlobal.pattr[channel].tolerance) {
            goto output;
        }
    } else {
        if (LimitGlobal.pretVal[channel] - val > LimitGlobal.pattr[channel].tolerance) {
            goto output;
        }
    }

    LimitGlobal.pbuffer[channel][LimitGlobal.peleIdx[channel]] = val;

    LimitGlobal.peleIdx[channel]++;
    if (LimitGlobal.peleIdx[channel] + 1 >= LimitGlobal.pattr[channel].eleCnt) {
        LimitGlobal.peleIdx[channel] = 0;
    }

output:

    LimitGlobal.pretVal[channel] = filter_limit_average(channel);
    return LimitGlobal.pretVal[channel];
}

sl_void filter_limit_exit(sl_void)
{
    sl_u8 channelIdx;

    for (channelIdx = 0; channelIdx < LimitGlobal.channelCnt; channelIdx++) {
        if (LimitGlobal.pbuffer[channelIdx]) {
            free(LimitGlobal.pbuffer[channelIdx]);
            LimitGlobal.pbuffer[channelIdx] = NULL;
        }
    }

    if (LimitGlobal.pattr) {
        free(LimitGlobal.pattr);
        LimitGlobal.pattr = NULL;
    }

    if (LimitGlobal.peleIdx) {
        free(LimitGlobal.peleIdx);
        LimitGlobal.peleIdx = NULL;
    }

    if (LimitGlobal.pretVal) {
        free(LimitGlobal.pretVal);
        LimitGlobal.pretVal = NULL;
    }

    return;
}
