#ifndef __FFJPEG_QUANT_H__
#define __FFJPEG_QUANT_H__

/* ȫ�ֱ������� */
extern int STD_QUANT_TAB[64];

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void init_fdct_ftab(int *ftab, int *qtab);
void init_idct_ftab(int *ftab, int *qtab);

#ifdef __cplusplus
}
#endif

#endif













