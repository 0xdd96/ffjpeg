#ifndef __FFJPEG_HUFFMAN_H__
#define __FFJPEG_HUFFMAN_H__

/* ����ͷ�ļ� */
#include "stdefine.h"

/* ���Ͷ��� */
/* ����������Ͷ��� */
typedef struct
{
    int symbol; /* ���� */
    int freq;   /* Ƶ�� */
    int group;  /* ���� */
    int depth;  /* �볤 */
    int code;   /* ���� */
} HUFCODEITEM;

/* bit stream ��д�����ӿ� */
typedef int  (*PFNCB_BITSTR_READ )(void *stream, int nbit);
typedef int  (*PFNCB_BITSTR_WRITE)(void *stream, int nbit, int data);

/* ���������Ͷ��� */
typedef struct
{
    BYTE               huftab[272];  /* ��������     - DE */
    int                first[18];    /* first        - DE */
    int                index[18];    /* index        - D  */
    HUFCODEITEM        codelist[256];/* �����       - E  */
    void              *input;        /* input bit stream  */
    void              *output;       /* output bit stream */
    PFNCB_BITSTR_READ  readbits;     /* read bits         */
    PFNCB_BITSTR_WRITE writebits;    /* write bits        */
} HUFCODEC;

#ifdef __cplusplus
extern "C" {
#endif

/* �������� */
/* ��׼�ķ���Ƶ��ͳ���� */
void huffman_stat_freq(HUFCODEITEM codelist[256], void *stream, PFNCB_BITSTR_READ readbits);


/* ���� */
/*
  ���뷽����
      1. ��ʼ�� HUFCODEC �е� freqlist ��Ա�����Բ���
         huffman_stat_freq ����Ƶ��ͳ�ƣ�Ҳ�����Լ���
         ��ʵ��Ƶ��ͳ��

      2. ���� huffman_encode_begin ������������ huftab,
         codelist �ֱ��Ӧ�˹������� (JPEG format)��
         �����

      3. ָ�� srcbitstr �� destbitstr �ֱ����������
         �е�Դ��������Ŀ��������

      4. ָ��������Ȼص�����ָ�� pps, ���ڱ�����ȿ�
         ��

      5. ���� huffman_encode_run ��������Զ��ı����
         ������Ľ��������� srcbitstr �� destbitstr ��
         ����β��

      6. ���� huffman_encode_done ��������������

      7. Ҳ���Բ�ͨ�� huffman_encode_run ���б��룬��
         huffman_encode_begin �Ժ��û������Լ���
         �� codelist ��ɱ������
 */
BOOL huffman_encode_begin(HUFCODEC *phc);
void huffman_encode_done (HUFCODEC *phc);
BOOL huffman_encode_run  (HUFCODEC *phc);

/* ���� */
/*
  ���뷽����
      1. ��ʼ�� HUFCODEC �е� huftab ��Ա��hufman
         �� JPEG format �ķ�ʽ��������

      2. ���� huffman_decode_begin ������� huftab
         ���� first �� index ������

      3. ָ�� srcbitstr �� destbitstr �ֱ�������
         �����е�Դ��������Ŀ��������

      4. ָ��������Ȼص�����ָ�� pps, ���ڽ����
         �ȿ���

      3. ���� huffman_decode_run ��������Զ��Ľ���
         ��������Ľ��������� srcbitstr �� destbitstr
         ������β��

      6. ���� huffman_decode_done ��������������

      7. Ҳ����ͨ�� huffman_decode_one ���е���
         �Ĺ���������������ú���ÿ�ε���ֻ�����
         �����н����һ������
 */
BOOL huffman_decode_begin(HUFCODEC *phc);
void huffman_decode_done (HUFCODEC *phc);
BOOL huffman_decode_run  (HUFCODEC *phc);

/* һ��ֻ�����һ������ */
int  huffman_decode_one  (HUFCODEC *phc);

#ifdef __cplusplus
}
#endif

#endif






