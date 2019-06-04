/*************************************************  
* Copyright (c) 2019
* All rights reserved.  
*   
* �ļ����ƣ�main.c  
* ��Ҫ���ܣ�n*m yuv420pͼ��ƴ��  
*      
* ԭ����  ��������
* ������ڣ�2019��03��25��  
*************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yuv420psplice.h"
#include "config.h"
#include "log.h"

const char* g_LogFileName = "log.txt";             // ��־�ļ�����ʼ��

int main(int argc, char* argv[])
{
	const char* pConfigFileName = "config.txt";    // �����ļ���

	SInputFileInfo sInputFileInfo;                 // �����ļ���Ϣ
	FILE** ppInputFile = NULL;                     // �����ļ�ָ������
	int* pFrameNum = NULL;                         // �����ļ�֡��Ŀ����
	byte_t* pInputBuf = NULL;                      // �����ļ�������
	
	byte_t* pOutputBuf = NULL;                     // ����ļ�������
	FILE* pOutputFile = NULL;                      // ����ļ�
	
	int nIsFrameNumSame = 0;                       // �ļ�֡��Ŀ�Ƿ���ͬ��־
	
	SOutYUVPosition* pOutYUV = NULL;               // �����ļ�������ļ���yuv����λ��
	SOutXYPosition* pOutXY = NULL;                 // �����ļ�������ļ���xy����λ��
	
	SOriginFileInfo sOriginFileInfo;               // ����ԭYUV420p�ļ��ߴ���Ϣ
	SResultFileInfo sResultFileInfo;               // ����ϳɽ��YUV420p�ļ��ߴ���Ϣ
	
	int i, j;                                      // ѭ�����Ʊ���

	// �ж������ļ��Ƿ����
	if ((fopen(pConfigFileName, "rb")) == NULL)
	{
		write_log(g_LogFileName, "ERROR: config file fopen failed.", __FILE__, __LINE__);
	    printf("ERROR: configFileName fopen failed.\n");
		getchar();
		exit(1);
	}
	// �ж���־�ļ��Ƿ����
	if ((fopen(g_LogFileName, "rb")) == NULL)
	{
		write_log(g_LogFileName, "ERROR: logFileName fopen failed.", __FILE__, __LINE__);
	    printf("ERROR: logFileName fopen failed.\n");
		getchar();
		exit(1);
	}

	// ���������ļ�
    load_config(pConfigFileName, &sInputFileInfo);
	// ��ʼ���ļ���Ϣ
	initialize_file_struct(&sOriginFileInfo, &sResultFileInfo, sInputFileInfo);

	// ����ռ�
	ppInputFile = (FILE**)malloc(sInputFileInfo.nInputNum * sizeof(FILE*));
	pFrameNum = (int*)malloc(sInputFileInfo.nInputNum * sizeof(int));
	pInputBuf = (byte_t*)malloc(sOriginFileInfo.nOriSize);
	pOutputBuf = (byte_t*)malloc(sResultFileInfo.nResSize);

	// �Ϸ��Լ��
	if (NULL == ppInputFile || NULL == pFrameNum || NULL == pInputBuf || NULL == pOutputBuf)
	{
		write_log(g_LogFileName, "ERROR: malloc failed.", __FILE__, __LINE__);
	    printf("ERROR: malloc failed.\n");
		getchar();
		exit(1);
	}
	else
	{
	    // ��ʼ��Ϊ0
		memset(ppInputFile, 0, sInputFileInfo.nInputNum * sizeof(FILE*));
		memset(pFrameNum, 0, sInputFileInfo.nInputNum * sizeof(int));
		memset(pInputBuf, 0, sOriginFileInfo.nOriSize);
		memset(pOutputBuf, 0, sResultFileInfo.nResSize);
	}
	
	// ��ȡ�����ļ�������ļ���xy����λ��
	pOutXY = get_input_file_xy_position(sInputFileInfo.nInputNum, sInputFileInfo.nOutputRow, sInputFileInfo.nOutputCol);
	// ��ȡ�����ļ�������ļ���yuv����λ��
	pOutYUV = get_output_file_yuv_position(pOutXY, sInputFileInfo.nInputNum, sOriginFileInfo, sResultFileInfo);
	
	// �������ļ�
	for (i = 0; i < sInputFileInfo.nInputNum; i++)
	{
	    if ((ppInputFile[i]=fopen(sInputFileInfo.szFilePath[INPUT_NUM_POSITION + i], "rb")) == NULL)
		{
			write_log(g_LogFileName, "ERROR: inputFile fopen failed.", __FILE__, __LINE__);
		    printf("ERROR: inputFile fopen failed.\n");
			getchar();
			exit(1);
		}
		else 
		{
			pFrameNum[i] = get_frame_num(ppInputFile[i], sOriginFileInfo.nOriSize);
		}
	}

	// ��������ļ�֡��Ŀ�Ƿ���ͬ
	nIsFrameNumSame = check_frame_num_is_same(pFrameNum, sInputFileInfo.nInputNum);
	if (-1 == nIsFrameNumSame)
	{
		write_log(g_LogFileName, "ERROR: frame num is not the same.", __FILE__, __LINE__);
	    printf("ERROR: frame num is not the same.\n");
		getchar();
		exit(1);
	}

	// ������ļ�
	if ((pOutputFile=fopen(sInputFileInfo.szFilePath[INPUT_NUM_POSITION + sInputFileInfo.nInputNum],"wb")) == NULL)
	{
		write_log(g_LogFileName, "outputFile open failed.", __FILE__, __LINE__);
	    printf("ERROR: outputFile open failed.\n");
		getchar();
		exit(1);
	}

	// yuv420p��Ƶƴ��
	yuv420p_splice(sInputFileInfo.nInputNum, pFrameNum[0], pOutYUV, pOutputFile,
			ppInputFile, pInputBuf, pOutputBuf, sOriginFileInfo, sResultFileInfo);

	// �ر��ļ�
	for (i = 0; i < sInputFileInfo.nInputNum; i++)
	{
	    fclose(ppInputFile[i]);
	}
	fclose(pOutputFile);

	// �ͷŶѿռ�
	free(ppInputFile);
	free(pInputBuf);
	free(pOutputBuf);
	free(pFrameNum);

	printf("yuv420p��Ƶƴ�ӳɹ���\n");

	system("pause");

	return 0;
}