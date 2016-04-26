/* ����ͷ�ļ� */
#include "huffman.h"

/* �ڲ�����ʵ�� */
/* ++ ���ڿ�������ıȽϺ��� */
static int cmp_freq_item(const HUFCODEITEM *a, const HUFCODEITEM *b)
{
    return a->freq - b->freq;
}

static int cmp_depth_item(const HUFCODEITEM *a, const HUFCODEITEM *b)
{
    return a->depth - b->depth;
}

static int cmp_symbol_item(const HUFCODEITEM *a, const HUFCODEITEM *b)
{
    return a->symbol - b->symbol;
}
/* -- ���ڿ�������ıȽϺ��� */

/* ����ÿ�����ŵĹ�����������볤 */
static void calculate_code_depth(HUFCODEITEM *codelist)
{
}

/* ����ʵ�� */
/* ͳ�Ʒ��Ŵ��и������ų��ֵ�Ƶ�� */
void huffman_stat_freq(HUFCODEITEM *codelist, void *stream, PFNCB_BITSTR_READ readbits)
{
    int  data;
    int  i;

    /* ��ʼ��Ƶ�ʱ� */
    for (i=0; i<256; i++)
    {
        codelist[i].symbol = i;
        codelist[i].freq   = 0;
    }

    /* ͳ��Ƶ�� */
    while (1) {
        data = readbits(stream, 8);
        if (data == -1) break;
        codelist[data].freq++;
    }
}

BOOL huffman_encode_begin(HUFCODEC *phc)
{
    return FALSE;
}

void huffman_encode_done (HUFCODEC *phc)
{
}

BOOL huffman_encode_run  (HUFCODEC *phc)
{
    return FALSE;
}

BOOL huffman_decode_begin(HUFCODEC *phc)
{
    return FALSE;
}

void huffman_decode_done (HUFCODEC *phc)
{
}

BOOL huffman_decode_run  (HUFCODEC *phc)
{
    return FALSE;
}

int  huffman_decode_one  (HUFCODEC *phc)
{
    return -1;
}











