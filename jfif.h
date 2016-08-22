#ifndef __FFJPEG_JFIF_H__
#define __FFJPEG_JFIF_H__

#ifdef __cplusplus
extern "C" {
#endif

/* ���Ͷ��� */
typedef (*PFN_JFIF_CB)(int seg_type, void *data, int len);

/* �������� */
void* jfif_load(char *file, PFN_JFIF_CB callback);
void  jfif_free(void *ctxt);

#ifdef __cplusplus
}
#endif

#endif

