#ifndef __FFJPEG_DCT_H__
#define __FFJPEG_DCT_H__

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
/* ��ά 8x8 �� DCT �任���� */
void fdct2d8x8(int *out, int *data);
void idct2d8x8(int *out, int *data);

#ifdef __cplusplus
}
#endif

#endif











