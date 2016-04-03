#ifndef __FFJPEG_RLE_H__
#define __FFJPEG_RLE_H__

/* ���Ͷ��� */
typedef struct
{
    int znum;
    int size;
    int code;
} RLEITEM, *PRLEITEM;

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void rle_encode(RLEITEM *dest, int *src, int *curdc);
void rle_decode(int *dest, RLEITEM *src, int *curdc);

#ifdef __cplusplus
}
#endif

#endif










