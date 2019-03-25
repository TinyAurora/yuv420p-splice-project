#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yuv420psplice.h"
#include "config.h"
#include "log.h"

extern const char* g_LogFileName;

// �����뻺����������䵽���������
void fill_output_buf(byte_t* pOutputBuf, int nOutStartPosition, int nOutOffset,    
	                 byte_t* pInputBuf, int nInStartPosion, int nInOffset,
					 int nWidthSize, int nHeightSize) 
{
	int h = 0;
	for (h = 0; h < nHeightSize; h++)
	{
	    memmove(pOutputBuf + nOutStartPosition + h * nOutOffset,
			    pInputBuf + nInStartPosion + h * nInOffset, nWidthSize);
	}
}

// ��������ļ�֡��Ŀ�Ƿ���ͬ
int check_frame_num_is_same(int* pFrameNum, int nSize)
{
	int i = 0;

	if (NULL == pFrameNum || nSize <= 0)
	{
		write_log(g_LogFileName, "ERROR: check_frame_num_is_same() failed.", __FILE__, __LINE__);
	    printf("ERROR: check_frame_num_is_same() failed.\n");
		getchar();
		exit(1);
	}
	
    for (i = 1; i < nSize; i++)
	{
	    if (pFrameNum[0] != pFrameNum[i])
		{
		    return -1;
		}
	}
	return 0;
}

// ��ȡ�ļ�֡��Ŀ
int get_frame_num(FILE* fp, int nOriSize)
{
    return (get_file_size(fp) / nOriSize);
}

// ��ȡ�ļ���С
int get_file_size(FILE* fp)
{
    int nFileSize = 0;
	
	if (NULL == fp) 
	{
		write_log(g_LogFileName, "ERROR: get_file_size() failed.", __FILE__, __LINE__);
	    printf("ERROR: get_file_size() failed");
		getchar();
		exit(1);
	}
	else
	{
	    fseek(fp, 0, SEEK_END);
		nFileSize = ftell(fp);
	}

	return nFileSize;
}

// ��ȡ�����ļ���������
SOutXYPosition* get_input_file_xy_position(int nFileNum, int nRow, int nCol)
{
	int i, j, k;
	SOutXYPosition* pOutXY = (SOutXYPosition*)malloc(nRow * nCol * sizeof(SOutXYPosition));

    if (nFileNum < 1 || nRow < 1 || nCol < 1 || nRow * nCol < nFileNum)
	{
		write_log(g_LogFileName, "ERROR: get_input_file_position() failed.", __FILE__, __LINE__);
	    printf("ERROR: get_input_file_position() failed.\n");
		getchar();
		exit(1);
	}

	i = 0;
	for (j = 0; j < nRow; j++)
	{
		for (k = 0; k < nCol; k++)
		{
			pOutXY[i].x = j;
			pOutXY[i].y = k;
			i++;
		}
	}
	return pOutXY;
}

// ��ȡ����ļ�YUV������������
SOutYUVPosition* get_output_file_yuv_position(SOutXYPosition* pOutXY, int nFileNum, 
	                                           SOriginFileInfo sOriginFileInfo, 
											   SResultFileInfo sResultFileInfo)
{
	// ����Y����λ��Ȩ��
	int nYWeightS = 0;
	int nYWeightH = sResultFileInfo.nResYWidth * sOriginFileInfo.nOriYHeight;
	int nYWeightW = sOriginFileInfo.nOriYWidth;
	// ����U����λ��Ȩ��
	int nUWeightS = sResultFileInfo.nResUOffset;
	int nUWeightH = sResultFileInfo.nResUWidth * sOriginFileInfo.nOriUHeight;
	int nUWeightW = sOriginFileInfo.nOriUWidth;
	// ����V����λ��Ȩ��
	int nVWeightS = sResultFileInfo.nResVOffset;
	int nVWeightH = sResultFileInfo.nResVWidth * sOriginFileInfo.nOriVHeight;
	int nVWeightW = sOriginFileInfo.nOriVWidth;

	int i = 0;

	SOutYUVPosition* pOutYUV = (SOutYUVPosition*)malloc(nFileNum * sizeof(SOutYUVPosition));

	if (NULL == pOutXY || nFileNum < 1)
	{
		write_log(g_LogFileName, "ERROR: get_output_file_yuv_position() failed.", __FILE__, __LINE__);
	    printf("ERROR: get_output_file_yuv_position() failed.");
		getchar();
		exit(1);
	}

	for (i = 0; i < nFileNum; i++)
	{
		pOutYUV[i].nOutYStartPosition = nYWeightS + nYWeightH * pOutXY[i].x + nYWeightW * pOutXY[i].y;
		pOutYUV[i].nOutUStartPosition = nUWeightS + nUWeightH * pOutXY[i].x + nUWeightW * pOutXY[i].y;
		pOutYUV[i].nOutVStartPosition = nVWeightS + nVWeightH * pOutXY[i].x + nVWeightW * pOutXY[i].y;
	}
	
	return pOutYUV;
}

// ��ȡÿ֡����
void read_each_frame_data(int nFileNum, int nInputFileFrameOffset, SOutYUVPosition* pOutYUV, 
	                      FILE* pOutputFile, FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo)
{
    int j = 0;

	for (j = 0; j < nFileNum; j++)
	{
		fseek(ppInputFile[j], nInputFileFrameOffset, SEEK_SET);
		fread(pInputBuf, 1, sOriginFileInfo.nOriSize, ppInputFile[j]);
						
		fill_output_buf(pOutputBuf, pOutYUV[j].nOutYStartPosition, sResultFileInfo.nResYWidth,
			            pInputBuf, 0, sOriginFileInfo.nOriYWidth, 
						sOriginFileInfo.nOriYWidth, sOriginFileInfo.nOriYHeight);
		fill_output_buf(pOutputBuf,  pOutYUV[j].nOutUStartPosition, sResultFileInfo.nResUWidth,
			            pInputBuf, sOriginFileInfo.nOriUOffset, sOriginFileInfo.nOriUWidth,
						sOriginFileInfo.nOriUWidth, sOriginFileInfo.nOriUHeight);
		fill_output_buf(pOutputBuf, pOutYUV[j].nOutVStartPosition, sResultFileInfo.nResVWidth,
			            pInputBuf, sOriginFileInfo.nOriVOffset, sOriginFileInfo.nOriVWidth,
				        sOriginFileInfo.nOriVWidth, sOriginFileInfo.nOriVHeight);
	}
	fwrite(pOutputBuf, 1, sResultFileInfo.nResSize, pOutputFile);
}

// yuv420p��Ƶƴ��
void yuv420p_splice(int nFileNum, int nFrameNum, SOutYUVPosition* pOutYUV, FILE* pOutputFile,
	                      FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo)
{
	int i = 0;
	int nInputFileFrameOffset = 0;
    // ѭ����ȡÿ֡����
	for (i = 0; i < nFrameNum; i++)
	{
		nInputFileFrameOffset = i * sOriginFileInfo.nOriSize;
		read_each_frame_data(nFileNum, nInputFileFrameOffset, pOutYUV, pOutputFile,
			ppInputFile, pInputBuf, pOutputBuf, sOriginFileInfo, sResultFileInfo);
	}
}


