#ifndef __FFJPEG_QUANT_H__
#define __FFJPEG_QUANT_H__

/* ȫ�ֱ������� */
extern int STD_QUANT_TAB[64];

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void quantize_encode(int *data, int *qtab);
void quantize_decode(int *data, int *qtab);

#ifdef __cplusplus
}
#endif

#endif













