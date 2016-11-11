#ifndef __FFJPEG_DCT_H__
#define __FFJPEG_DCT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ȫ�ֱ������� */
extern int FDCT_FACTOR_TAB[64];
extern int IDCT_FACTOR_TAB[64];

/* �������� */
/* ��ά 8x8 �� DCT �任���� */
void fdct2d8x8(int *du, int *ftab);
void idct2d8x8(int *du, int *ftab);

#ifdef __cplusplus
}
#endif

#endif











