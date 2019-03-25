#ifndef _YUVSPLICE_H
#define _YUVSPLICE_H

#include "config.h"

//// ������Ϣ�����������С
//#define BUF_ROW_SIZE 20
//#define BUF_COL_SIZE 256

typedef unsigned char byte_t;

// ��������ļ�YUV������������ṹ��
typedef struct SOutYUVPosition
{
	int nOutYStartPosition;
	int nOutUStartPosition;
	int nOutVStartPosition;
} SOutYUVPosition;

// ���������ļ�����λ�ýṹ��
typedef struct SOutXYPosition
{
   int x;
   int y;
} SOutXYPosition;

// ��ȡ�ļ���С
int get_file_size(FILE* fp);  
// ��ȡ�ļ�֡��Ŀ
int get_frame_num(FILE* fp, int nOriSize); 
// ��������ļ�֡��Ŀ�Ƿ���ͬ
int check_frame_num_is_same(int* pFrameNum, int nSize); 
// �����뻺����������䵽���������
void fill_output_buf(byte_t* pOutputBuf, int nOutStartPosition, int nOutOffset,    
	                 byte_t* pInputBuf, int nInStartPosion, int nInOffset,
					 int nWidthSize, int nHeightSize);
// ��ȡ�����ļ���������
SOutXYPosition* get_input_file_xy_position(int nFileNum, int nRow, int nCol);
// ��ȡ����ļ�YUV������������
SOutYUVPosition* get_output_file_yuv_position(SOutXYPosition* pOutXY, int nFileNum, 
	                                           SOriginFileInfo sOriginFileInfo, 
											   SResultFileInfo sResultFileInfo);
// ��ȡÿ֡����
void read_each_frame_data(int nFileNum, int nInputFileFrameOffset, SOutYUVPosition* pOutYUV, 
	                      FILE* pOutputFile, FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo);
// yuv420p��Ƶƴ��
void yuv420p_splice(int nFileNum, int nFrameNum, SOutYUVPosition* pOutYUV, FILE* pOutputFile,
	                      FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo);



#endif

