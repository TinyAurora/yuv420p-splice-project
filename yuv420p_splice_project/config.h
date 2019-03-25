#ifndef _CONFIG_H
#define _CONFIG_H

// 定义输出文件路径名在配置文件中排在所有输入文件后面
#define OUTPUT_POSITION 1
// 定义配置文件中程序启动标志位置
#define PROCEDURE_ON_POSITION 1
// 定义配置文件中输入图片宽、高位置
#define INPUT_FILE_WIDTH_POSITION 2
#define INPUT_FILE_HEIGHT_POSITION 3
// 定义配置文件中输出文件行、列位置
#define OUTPUT_ROW_POSITION 4
#define OUTPUT_COL_POSITION 5
// 定义文件数量在配置文件中为第六行，固定不可修改
#define INPUT_NUM_POSITION 6

// 定义输入文件信息结构体
typedef struct SInputFileInfo
{
    int nOriWidth;      // 输入文件宽
	int nOriHeight;     // 输入文件高
	int nOutputRow;     // 输出文件行数
	int nOutputCol;     // 输出文件列数
	int nInputNum;      // 输入文件数
	char szFilePath[50][256];    // 文件路径数组
} SInputFileInfo;

// 定义原YUV420p文件尺寸信息
typedef struct SOriginFileInfo
{
    // 原YUV420p图像尺寸大小
	int nOriWidth;
	int nOriHeight;
	int nOriSize;

	// 原YUV420p图像Y、U、V分量的宽高
	int nOriYWidth;
	int nOriYHeight;
	int nOriUWidth;
	int nOriUHeight;
	int nOriVWidth;
	int nOriVHeight;

	// U、V分量在原图像中的相对起始位置
	int nOriUOffset;
	int nOriVOffset;
} SOriginFileInfo;

// 定义合成结果YUV420p文件尺寸信息
typedef struct SResultFileInfo
{
    // 合成结果YUV420p图像尺寸大小
	int nResWidth;
	int nResHeight;
	int nResSize;

	// 合成结果YUV420p图像Y、U、V分量的宽高
	int nResYWidth;
	int nResYHeight;
	int nResUWidth;
	int nResUHeight;
	int nResVWidth;
	int nResVHeight;

	// U、V分量在合成后图像中的相对起始位置
	int nResUOffset;
	int nResVOffset;
} SResultFileInfo;

// 加载配置文件
void load_config(const char* pFilePath, SInputFileInfo* sInputFileInfo);
// 初始化文件信息
void initialize_file_struct(SOriginFileInfo* sOriginFileInfo, SResultFileInfo* sResultFileInfo, SInputFileInfo sInputFileInfo);



#endif