/* ����ͷ�ļ� */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "huffman.h"

#define ENABLE_DUMP_CODE_LIST  1

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

static int cmp_symbol_item(const void *a, const void *b)
{
    return ((HUFCODEITEM*)a)->symbol - ((HUFCODEITEM*)b)->symbol;
}
/* -- ���ڿ�������ıȽϺ��� */

#if ENABLE_DUMP_CODE_LIST
static void dump_huffman_codelist(char *title, HUFCODEITEM *list, int n, int head)
{
    int i;
    printf("\n+----%s----+\n", title);
    for (i=0; i<n; i++) {
        printf("%c %c, freq:%3d, group:%3d, depth:%d\n",
            head == i ? '*' : ' ',
            list[i].symbol, list[i].freq, list[i].group, list[i].depth);
    }
}
#endif

/* ����ÿ�����ŵĹ�����������볤 */
static void calculate_code_depth(HUFCODEITEM *codelist)
{
    HUFCODEITEM  copylist[256];
    HUFCODEITEM *templist;
    int head;
    int group;
    int n, i;

    /* make a copylist which is copy of codelist */
    memcpy(copylist, codelist, sizeof(HUFCODEITEM) * 256);

    /* ��Ƶ�ʱ���п������� */
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
#if ENABLE_DUMP_CODE_LIST
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

#if ENABLE_DUMP_CODE_LIST
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

#if ENABLE_DUMP_CODE_LIST
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

#if ENABLE_DUMP_CODE_LIST
    // dump done code list
    dump_huffman_codelist(" done ---", templist, n, head);
#endif

    for (i=0; i<n; i++) {
        codelist[templist[i].symbol].depth = templist[i].depth;
    }
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
        codelist[i].group  = i;
        codelist[i].depth  = 0;
        codelist[i].code   = 0;
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


#if 0
int main(void)
{
    HUFCODEITEM codelist[256] = {0};
    codelist['H'].symbol = 'H';
    codelist['H'].freq   = 1;
    codelist['H'].group  = 'H';

    codelist['F'].symbol = 'F';
    codelist['F'].freq   = 2;
    codelist['F'].group  = 'F';

    codelist['C'].symbol = 'C';
    codelist['C'].freq   = 3;
    codelist['C'].group  = 'C';

    codelist['J'].symbol = 'J';
    codelist['J'].freq   = 4;
    codelist['J'].group  = 'J';

    codelist['I'].symbol = 'I';
    codelist['I'].freq   = 5;
    codelist['I'].group  = 'I';

    codelist['D'].symbol = 'D';
    codelist['D'].freq   = 7;
    codelist['D'].group  = 'D';

    codelist['A'].symbol = 'A';
    codelist['A'].freq   = 8;
    codelist['A'].group  = 'A';

    codelist['E'].symbol = 'E';
    codelist['E'].freq   = 9;
    codelist['E'].group  = 'E';

    codelist['K'].symbol = 'K';
    codelist['K'].freq   = 9;
    codelist['K'].group  = 'K';

    codelist['B'].symbol = 'B';
    codelist['B'].freq   = 16;
    codelist['B'].group  = 'B';

    codelist['G'].symbol = 'G';
    codelist['G'].freq   = 32;
    codelist['G'].group  = 'G';

    calculate_code_depth(codelist);

    return 0;
}
#endif








