#ifndef __FFJPEG_ZIGZAG_H__
#define __FFJPEG_ZIGZAG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void zigzag_encode(int *data);
void zigzag_decode(int *data);

#ifdef __cplusplus
}
#endif

#endif








