#ifndef __FFJPEG_RLE_H__
#define __FFJPEG_RLE_H__

/* ���Ͷ��� */
typedef struct
{
    int size;
    int runlen;
    int ampcode;
} RLEITEM, *PRLEITEM;

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void RLEEncode(RLEITEM *dest, int *src, int *curdc);
void RLEDecode(int *dest, RLEITEM *src, int *curdc);

#ifdef __cplusplus
}
#endif

#endif










