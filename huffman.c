/* ����ͷ�ļ� */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "bitstr.h"
#include "huffman.h"

#define ENABLE_DEBUG_DUMP  1

/* �ڲ�����ʵ�� */
/* ++ ���ڿ�������ıȽϺ��� */
static int cmp_freq_item(const void *a, const void *b)
{
    return ((HUFCODEITEM*)a)->freq - ((HUFCODEITEM*)b)->freq;
}

static int cmp_depth_item(const void *a, const void *b)
{
    return ((HUFCODEITEM*)a)->depth - ((HUFCODEITEM*)b)->depth;
}
/* -- ���ڿ�������ıȽϺ��� */

#if ENABLE_DEBUG_DUMP
static void int_to_bin_str(int v, char *str, int n) {
    int i;
    for (i=n-1; i>=0; i--) {
        if (v & (1 << i)) {
            *str++ = '1';
        }
        else {
            *str++ = '0';
        }
    }
    *str = '\0';
}

static void dump_huffman_codelist(char *title, HUFCODEITEM *list, int n, int head)
{
    char str[33];
    int  i;
    printf("\n+----%s----+\n", title);
    for (i=0; i<n; i++) {
        int_to_bin_str(list[i].code, str, list[i].depth);
        printf("%c %c, freq:%3d, group:%3d, depth:%d, code:%s\n",
            head == i ? '*' : ' ',
            list[i].symbol, list[i].freq, list[i].group, list[i].depth, str);
    }
}
#endif

/* ����ʵ�� */
/* ͳ�Ʒ��Ŵ��и������ų��ֵ�Ƶ�� */
void huffman_stat_freq(HUFCODEITEM codelist[256], void *stream)
{
    int  data;
    int  i;

    /* ��ʼ��Ƶ�ʱ� */
    for (i=0; i<256; i++)
    {
        codelist[i].symbol = i;
        codelist[i].freq   = 0;
        codelist[i].group  = i;
        codelist[i].depth  = 1;
        codelist[i].code   = 0;
    }

    // ����Ƶ�ʣ��������ֵ���������ʹ���
    #define MAX_STAT_FREQ_NUM  (1ul << (MAX_HUFFMAN_CODE_LEN - 1))

    /* ͳ��Ƶ�� */
    while (1) {
        data = bitstr_getc(stream);
        if (data == EOF) break;
        if (codelist[(BYTE)data].freq < MAX_STAT_FREQ_NUM) {
            codelist[(BYTE)data].freq++;
        }
    }

#if ENABLE_DEBUG_DUMP
    printf("\nstat freq:\n");
    printf("------------\n");
    for (i=0; i<256; i++) {
        if (codelist[i].freq) {
            printf("%c:%d ", codelist[i].symbol, codelist[i].freq);
        }
    }
    printf("\n");
#endif
}

BOOL huffman_encode_begin(HUFCODEC *phc)
{
    HUFCODEITEM *codelist = phc->codelist;
    HUFCODEITEM  copylist[256];
    HUFCODEITEM *templist;
    BYTE        *huftab   = phc->huftab;
    int head;
    int group;
    int code;
    int n, i, j, k;

    /* make a copylist which is copy of codelist */
    memcpy(copylist, codelist, sizeof(HUFCODEITEM) * 256);

    /* �� copylist �� freq ���п������� */
    qsort(copylist, 256, sizeof(HUFCODEITEM), cmp_freq_item);

    /* ���ҳ���һ������Ƶ�ʵķ���
       �����������Ƶ�ʵķ��Ÿ��� */
    for (i=0; i<256 && !copylist[i].freq; i++);
    templist = &copylist[i];
    n = 256 - i;

    /* ��ʼ��ͷָ��͵�ǰ���� */
    head  = 0;
    group = 256;

    while (head < n - 2) /* ��һ�� while ѭ���м����볤 */
    {
#if ENABLE_DEBUG_DUMP
        // dump sorted code list
        dump_huffman_codelist(" sorted -", templist, n, head);
#endif

        /* ���� templist �� head ��ʼ������Ԫ�صķ������
           ������ templist �� head ֮ǰ��ӦԪ�ص��볤�ͷ��� */
        for (i=0; i<head; i++)
        {
            if (  templist[i].group == templist[head + 0].group
               || templist[i].group == templist[head + 1].group)
            {
                templist[i].depth++;       /* �볤ֵ�� 1 */
                templist[i].group = group; /* �����µķ��� */
            }
        }

#if ENABLE_DEBUG_DUMP
        // dump updated code list
        dump_huffman_codelist(" updated ", templist, n, head);
#endif

        /* �ϲ�Ƶ����С�������� */
        templist[head + 0].depth++;
        templist[head + 1].depth++;
        templist[head + 0].group = group;
        templist[head + 1].group = group;
        templist[head + 1].freq += templist[head + 0].freq;

        head ++; /* ��ͷָ�� */
        group++; /* ������ */

#if ENABLE_DEBUG_DUMP
        // dump merged code list
        dump_huffman_codelist(" merged -", templist, n, head);
#endif

        /* �� templist �������� */
        for (i=head; i<n-1; i++)
        {
            if (templist[i].freq > templist[i+1].freq)
            {
                HUFCODEITEM tempitem;
                memcpy(&tempitem,        &(templist[i+0]), sizeof(HUFCODEITEM));
                memcpy(&(templist[i+0]), &(templist[i+1]), sizeof(HUFCODEITEM));
                memcpy(&(templist[i+1]), &tempitem,        sizeof(HUFCODEITEM));
            }
            else break;
        }
    }

    /* �� templist �� depth ���п������� */
    qsort(templist, n, sizeof(HUFCODEITEM), cmp_depth_item);

#if ENABLE_DEBUG_DUMP
    // dump done code list
    dump_huffman_codelist(" done ---", templist, n, -1);
#endif

    // ���� jpeg ��ʽ�Ĺ�������
    memset(huftab, 0, sizeof(huftab));
    for (i=0; i<n; i++) {
        if (huftab[templist[i].depth - 1] < 0xff) {
            huftab[templist[i].depth - 1]++;
        }
        huftab[i + MAX_HUFFMAN_CODE_LEN] = templist[i].symbol;
    }

#if ENABLE_DEBUG_DUMP
    printf("\nhuftab:\n");
    for (i=0; i<MAX_HUFFMAN_CODE_LEN; i++) {
        printf("%d ", huftab[i]);
    }
    printf("\n");
    for (i=MAX_HUFFMAN_CODE_LEN; i<MAX_HUFFMAN_CODE_LEN+256; i++) {
        printf("%c ", huftab[i]);
    }
    printf("\n");
#endif

    k    = 0;
    code = 0;
    for (j=templist[0].depth-1; j<MAX_HUFFMAN_CODE_LEN; j++) {
        for (i=0; i<huftab[j]; i++) {
            templist[k++].code = code++;
        }
        code <<= 1;
    }

#if ENABLE_DEBUG_DUMP
    for (i=0; i<n; i++) {
        templist[i].freq  = codelist[templist[i].symbol].freq ;
        templist[i].group = codelist[templist[i].symbol].group;
    }
    // dump done code list
    dump_huffman_codelist(" code ---", templist, n, -1);
#endif

    for (i=0; i<n; i++) {
        codelist[templist[i].symbol].depth = templist[i].depth;
        codelist[templist[i].symbol].code  = templist[i].code ;
    }

    return TRUE;
}

void huffman_encode_done(HUFCODEC *phc)
{
    /* flush bitstr */
    bitstr_flush(phc->output);
}

BOOL huffman_encode_run(HUFCODEC *phc)
{
    /* ��������������������Ч�� */
    if (!phc->input || !phc->output) return FALSE;

#if ENABLE_DEBUG_DUMP
    printf("\noutput encode bit stream:\n");
#endif

    /* �������������б��벢��� */
    while (1) {
        int data = bitstr_getc(phc->input);
        if (data == EOF) break;

        {
            int code, i;
            code = phc->codelist[data].code;
            for (i=phc->codelist[data].depth-1; i>=0; i--) {
                if (EOF == bitstr_putb((code & (1 << i)) ? 1 : 0, phc->output)) {
                    return FALSE;
                }
#if ENABLE_DEBUG_DUMP
                printf("%d", (code & (1 << i)) ? 1 : 0);
#endif
            }
        }
    }

    /* ���سɹ� */
    return TRUE;
}

BOOL huffman_decode_begin(HUFCODEC *phc)
{
    int i;

    /* ���ݹ��������� first ��� index ��
       first[i] ��ʾ����Ϊ i+1 �ĵ�һ�����ֵ�ֵ
       index[i] ��ʾ����Ϊ i+1 �ĵ�һ�����ֵ����� */
    phc->first[0] = 0 ;
    phc->index[0] = MAX_HUFFMAN_CODE_LEN;
    for (i=1; i<MAX_HUFFMAN_CODE_LEN; i++)
    {
        phc->first[i] = (phc->first[i-1] + phc->huftab[i-1]) << 1;
        phc->index[i] =  phc->index[i-1] + phc->huftab[i-1];
    }

#if ENABLE_DEBUG_DUMP
    printf("\n\nfirst table:\n");
    for (i=0; i<MAX_HUFFMAN_CODE_LEN; i++) {
        printf("%d ", phc->first[i]);
    }

    printf("\n\nindex table:\n");
    for (i=0; i<MAX_HUFFMAN_CODE_LEN; i++) {
        printf("%d ", phc->index[i]);
    }
    printf("\n\n");
#endif

    /* ���سɹ� */
    return TRUE;
}

void huffman_decode_done(HUFCODEC *phc)
{
    /* flush bitstr */
    bitstr_flush(phc->output);
}

BOOL huffman_decode_run(HUFCODEC *phc)
{
    int symbol;

    /* ��������������������Ч�� */
    if (!phc->input || !phc->output) return FALSE;

    /* decode until end of stream */
    while (1) {
        symbol = huffman_decode_one(phc);
        if (symbol == EOF) {
            printf("get the EOF from huffman decoder !\n");
            break;
        }
        if (EOF == bitstr_putc(symbol, phc->output)) {
            printf("failed to write data to bit stream !\n");
            return FALSE;
        }
    }

    /* ���سɹ� */
    return TRUE;
}

int huffman_decode_one(HUFCODEC *phc)
{
    int bit;
    int code = 0;
    int len  = 0;
    int idx  = 0;

    /* ��������������������Ч�� */
    if (!phc->input) return EOF;

    /* ����������ȡ���� */
    while (1) {
        bit = bitstr_getb(phc->input);
        if ( bit == EOF) return EOF;
        printf("%d, first = %d, len = %d\n", bit ? 1 : 0, phc->first[len], len);
        code <<= 1; code |= bit;
        if (code - phc->first[len] < phc->huftab[len]) break;
        if (++len == MAX_HUFFMAN_CODE_LEN) return EOF;
    }

    idx = phc->index[len] + (code - phc->first[len]);
    printf("get code:%c len:%d, idx:%d\n\n", phc->huftab[idx], len, idx);
    return idx < MAX_HUFFMAN_CODE_LEN + 256 ? phc->huftab[idx] : EOF;
}


#if ENABLE_DEBUG_DUMP
int main(void)
{
    HUFCODEC hufencoder;
    HUFCODEC hufdecoder;

    //++ encode test
    hufencoder.input  = bitstr_open("test.txt" , "rb");
    hufencoder.output = bitstr_open("test.huf", "wb");

    huffman_stat_freq(hufencoder.codelist, hufencoder.input);
    bitstr_seek(hufencoder.input, SEEK_SET, 0);

    huffman_encode_begin(&hufencoder);
    huffman_encode_run  (&hufencoder);
    huffman_encode_done (&hufencoder);

    bitstr_close(hufencoder.input );
    bitstr_close(hufencoder.output);
    //-- encode test

    //++ decode test
    memcpy(hufdecoder.huftab, hufencoder.huftab, sizeof(hufdecoder.huftab));

    hufdecoder.input  = bitstr_open("test.huf"  , "rb");
    hufdecoder.output = bitstr_open("decode.txt", "wb");

    huffman_decode_begin(&hufdecoder);
    huffman_decode_run  (&hufdecoder);
    huffman_decode_done (&hufdecoder);

    bitstr_close(hufdecoder.input );
    bitstr_close(hufdecoder.output);
    //-- decode test

    return 0;
}
#endif






