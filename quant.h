#ifndef __FFJPEG_QUANT_H__
#define __FFJPEG_QUANT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ȫ�ֱ������� */
extern const int STD_QUANT_TAB_LUMIN[64];
extern const int STD_QUANT_TAB_CHROM[64];

/* �������� */
void init_fdct_ftab(int *ftab, int *qtab);
void init_idct_ftab(int *ftab, int *qtab);

#ifdef __cplusplus
}
#endif

#endif













