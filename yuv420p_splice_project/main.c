/*************************************************  
* Copyright (c) 2019
* All rights reserved.  
*   
* 文件名称：main.c  
* 主要功能：n*m yuv420p图像拼接  
*      
* 原作者  ：陈晓明
* 完成日期：2019年03月25日  
*************************************************/   

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yuv420psplice.h"
#include "config.h"
#include "log.h"

const char* g_LogFileName = "log.txt";             // 日志文件名初始化

int main(int argc, char* argv[])
{
	const char* pConfigFileName = "config.txt";    // 配置文件名

	SInputFileInfo sInputFileInfo;                 // 输入文件信息
	FILE** ppInputFile = NULL;                     // 输入文件指针数组
	int* pFrameNum = NULL;                         // 输入文件帧数目数组
	byte_t* pInputBuf = NULL;                      // 输入文件缓冲区
	
	byte_t* pOutputBuf = NULL;                     // 输出文件缓冲区
	FILE* pOutputFile = NULL;                      // 输出文件
	
	int nIsFrameNumSame = 0;                       // 文件帧数目是否相同标志
	
	SOutYUVPosition* pOutYUV = NULL;               // 输入文件在输出文件的yuv分量位置
	SOutXYPosition* pOutXY = NULL;                 // 输入文件在输出文件的xy坐标位置
	
	SOriginFileInfo sOriginFileInfo;               // 定义原YUV420p文件尺寸信息
	SResultFileInfo sResultFileInfo;               // 定义合成结果YUV420p文件尺寸信息
	
	int i, j;                                      // 循环控制变量

	// 判断配置文件是否存在
	if ((fopen(pConfigFileName, "rb")) == NULL)
	{
		write_log(g_LogFileName, "ERROR: config file fopen failed.", __FILE__, __LINE__);
	    printf("ERROR: configFileName fopen failed.\n");
		getchar();
		exit(1);
	}
	// 判断日志文件是否存在
	if ((fopen(g_LogFileName, "rb")) == NULL)
	{
		write_log(g_LogFileName, "ERROR: logFileName fopen failed.", __FILE__, __LINE__);
	    printf("ERROR: logFileName fopen failed.\n");
		getchar();
		exit(1);
	}

	// 加载配置文件
    load_config(pConfigFileName, &sInputFileInfo);
	// 初始化文件信息
	initialize_file_struct(&sOriginFileInfo, &sResultFileInfo, sInputFileInfo);

	// 分配空间
	ppInputFile = (FILE**)malloc(sInputFileInfo.nInputNum * sizeof(FILE*));
	pFrameNum = (int*)malloc(sInputFileInfo.nInputNum * sizeof(int));
	pInputBuf = (byte_t*)malloc(sOriginFileInfo.nOriSize);
	pOutputBuf = (byte_t*)malloc(sResultFileInfo.nResSize);

	// 合法性检查
	if (NULL == ppInputFile || NULL == pFrameNum || NULL == pInputBuf || NULL == pOutputBuf)
	{
		write_log(g_LogFileName, "ERROR: malloc failed.", __FILE__, __LINE__);
	    printf("ERROR: malloc failed.\n");
		getchar();
		exit(1);
	}
	else
	{
	    // 初始化为0
		memset(ppInputFile, 0, sInputFileInfo.nInputNum * sizeof(FILE*));
		memset(pFrameNum, 0, sInputFileInfo.nInputNum * sizeof(int));
		memset(pInputBuf, 0, sOriginFileInfo.nOriSize);
		memset(pOutputBuf, 0, sResultFileInfo.nResSize);
	}
	
	// 获取输入文件在输出文件的xy坐标位置
	pOutXY = get_input_file_xy_position(sInputFileInfo.nInputNum, sInputFileInfo.nOutputRow, sInputFileInfo.nOutputCol);
	// 获取输入文件在输出文件的yuv分量位置
	pOutYUV = get_output_file_yuv_position(pOutXY, sInputFileInfo.nInputNum, sOriginFileInfo, sResultFileInfo);
	
	// 打开输入文件
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

	// 检查输入文件帧数目是否相同
	nIsFrameNumSame = check_frame_num_is_same(pFrameNum, sInputFileInfo.nInputNum);
	if (-1 == nIsFrameNumSame)
	{
		write_log(g_LogFileName, "ERROR: frame num is not the same.", __FILE__, __LINE__);
	    printf("ERROR: frame num is not the same.\n");
		getchar();
		exit(1);
	}

	// 打开输出文件
	if ((pOutputFile=fopen(sInputFileInfo.szFilePath[INPUT_NUM_POSITION + sInputFileInfo.nInputNum],"wb")) == NULL)
	{
		write_log(g_LogFileName, "outputFile open failed.", __FILE__, __LINE__);
	    printf("ERROR: outputFile open failed.\n");
		getchar();
		exit(1);
	}

	// yuv420p视频拼接
	yuv420p_splice(sInputFileInfo.nInputNum, pFrameNum[0], pOutYUV, pOutputFile,
			ppInputFile, pInputBuf, pOutputBuf, sOriginFileInfo, sResultFileInfo);

	// 关闭文件
	for (i = 0; i < sInputFileInfo.nInputNum; i++)
	{
	    fclose(ppInputFile[i]);
	}
	fclose(pOutputFile);

	// 释放堆空间
	free(ppInputFile);
	free(pInputBuf);
	free(pOutputBuf);
	free(pFrameNum);

	printf("yuv420p视频拼接成功！\n");

	system("pause");

	return 0;
}