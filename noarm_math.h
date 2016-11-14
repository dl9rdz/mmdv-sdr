#ifndef NOARM_MATH_H
#define NOARM_MATH_H
#include <stdint.h>
#include <stddef.h>
#define __SSAT(a,b) ( (a)>=(1<<(b-1))?(1<<(b-1))-1 : (a)<-(1<<(b-1)) ? -(1<<(b-1)) : a )
typedef int16_t q15_t;
typedef int32_t q31_t;
typedef int64_t q63_t;
typedef struct
{
    uint16_t numTaps;
    q15_t *pState;
    q15_t *pCoeffs;
} arm_fir_instance_q15;
void arm_fir_fast_q15(
        const arm_fir_instance_q15 * S,
        q15_t * pSrc,
        q15_t * pDst,
        uint32_t blockSize);
#endif
