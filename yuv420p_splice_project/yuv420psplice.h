#ifndef _YUVSPLICE_H
#define _YUVSPLICE_H

#include "config.h"

//// 定义信息缓冲区数组大小
//#define BUF_ROW_SIZE 20
//#define BUF_COL_SIZE 256

typedef unsigned char byte_t;

// 定义输出文件YUV分量放置坐标结构体
typedef struct SOutYUVPosition
{
	int nOutYStartPosition;
	int nOutUStartPosition;
	int nOutVStartPosition;
} SOutYUVPosition;

// 定义输入文件坐标位置结构体
typedef struct SOutXYPosition
{
   int x;
   int y;
} SOutXYPosition;

// 获取文件大小
int get_file_size(FILE* fp);  
// 获取文件帧数目
int get_frame_num(FILE* fp, int nOriSize); 
// 检查输入文件帧数目是否相同
int check_frame_num_is_same(int* pFrameNum, int nSize); 
// 将输入缓冲区内容填充到输出缓冲区
void fill_output_buf(byte_t* pOutputBuf, int nOutStartPosition, int nOutOffset,    
	                 byte_t* pInputBuf, int nInStartPosion, int nInOffset,
					 int nWidthSize, int nHeightSize);
// 获取输入文件放置坐标
SOutXYPosition* get_input_file_xy_position(int nFileNum, int nRow, int nCol);
// 获取输出文件YUV分量放置坐标
SOutYUVPosition* get_output_file_yuv_position(SOutXYPosition* pOutXY, int nFileNum, 
	                                           SOriginFileInfo sOriginFileInfo, 
											   SResultFileInfo sResultFileInfo);
// 读取每帧数据
void read_each_frame_data(int nFileNum, int nInputFileFrameOffset, SOutYUVPosition* pOutYUV, 
	                      FILE* pOutputFile, FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo);
// yuv420p视频拼接
void yuv420p_splice(int nFileNum, int nFrameNum, SOutYUVPosition* pOutYUV, FILE* pOutputFile,
	                      FILE** ppInputFile, byte_t* pInputBuf, byte_t* pOutputBuf,
						  SOriginFileInfo sOriginFileInfo, SResultFileInfo sResultFileInfo);



#endif

