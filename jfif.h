#ifndef __FFJPEG_JFIF_H__
#define __FFJPEG_JFIF_H__

// ����ͷ�ļ�
#include "bmp.h"

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
void* jfif_load(char *file);
int   jfif_save(void *ctxt, char *file);
void  jfif_free(void *ctxt);

int jfif_decode(void *ctxt, BMP *pb);
int jfif_encode(void *ctxt, BMP *pb);

#ifdef __cplusplus
}
#endif

#endif

