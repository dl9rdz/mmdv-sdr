/* ----------------------------------------------------------------------   
* simple temporary replacement for arm_fir_fast_q15
* TODO: check license
* hr
* -------------------------------------------------------------------- */

//#include "arm_math.h"
#include "Config.h"
#include "Globals.h"


void arm_fir_fast_q15(
  const arm_fir_instance_q15 * S,
  q15_t * pSrc,
  q15_t * pDst,
  uint32_t blockSize)
{
  q15_t *pState = S->pState;
  q15_t *pCoeffs = S->pCoeffs;
  q15_t *pStateCurnt; 

  q15_t *px;
  q15_t *pb;
  q63_t acc;
  uint32_t numTaps = S->numTaps;
  uint32_t tapCnt, blkCnt;

  pStateCurnt = &(S->pState[(numTaps - 1u)]);
  blkCnt = blockSize;

  while(blkCnt > 0u) {
    *pStateCurnt++ = *pSrc++;
    acc = 0;
    px = pState;
    pb = pCoeffs;
    tapCnt = numTaps;
    // multiply-accumulate
    do
    {
      acc += (q31_t) * px++ * *pb++;
      tapCnt--;
    } while(tapCnt > 0u);

    /* convert 2.30 to 1.15 with saturation and store result in destination buffer */
    *pDst++ = (q15_t) __SSAT((acc >> 15u), 16);

    pState = pState + 1;
    blkCnt--;
  }

  pStateCurnt = S->pState;
  tapCnt = (numTaps - 1u);
  while(tapCnt > 0u) {
    *pStateCurnt++ = *pState++;
    tapCnt--;
  }
}
