#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "yuv420psplice.h"
#include "config.h"
#include "log.h"

extern const char* g_LogFileName;

// ���������ļ�
void load_config(const char* pFilePath, SInputFileInfo* sInputFileInfo)
{
    FILE* fp = fopen(pFilePath, "r");
	char szBuf[256] = {0};
	char szKeyWord[50][256] = {0};
	int nInfoNum = 0;
	int i;
	unsigned int j;

	// ���=������ַ�����Ϣ
	i = 0;
	while (fgets(szBuf, sizeof(szBuf), fp) != NULL)
	{
		// �ж�fgets�Ƿ�����쳣
		if (ferror(fp) != 0)
		{
		    write_log(g_LogFileName, "ERROR: fgets() failed.", __FILE__, __LINE__);
			printf("ERROR: fgets() failed.\n");
			getchar();
			exit(1);
		}

		// ��ȡ=��ǰ��ؼ���
		strncpy(szKeyWord[i], szBuf, strlen(szBuf) - strlen(strrchr(szBuf, '=')));

		// ��ȡ=�ź���ؼ���
		sprintf((*sInputFileInfo).szFilePath[i], "%s", strrchr(szBuf, '=') + 1);

		i++;
	}

	nInfoNum = atoi((*sInputFileInfo).szFilePath[INPUT_NUM_POSITION - 1]) + INPUT_NUM_POSITION;

	// ȥ����β�Ļ��з�
	for (i = 0; i < nInfoNum; i++)
	{
		for (j = 0; j < strlen((*sInputFileInfo).szFilePath[i]); j++)
		{
		    if ((*sInputFileInfo).szFilePath[i][j] == '\n')
			{
			    (*sInputFileInfo).szFilePath[i][j] = '\0';
			}
		}
	}

	/*(*sInputFileInfo).nOriWidth = atoi((*sInputFileInfo).szFilePath[INPUT_FILE_WIDTH_POSITION - 1]);
	(*sInputFileInfo).nOriHeight = atoi((*sInputFileInfo).szFilePath[INPUT_FILE_HEIGHT_POSITION - 1]);
	(*sInputFileInfo).nOutputRow = atoi((*sInputFileInfo).szFilePath[OUTPUT_ROW_POSITION - 1]);
	(*sInputFileInfo).nOutputCol = atoi((*sInputFileInfo).szFilePath[OUTPUT_COL_POSITION - 1]);
	(*sInputFileInfo).nInputNum = atoi((*sInputFileInfo).szFilePath[INPUT_NUM_POSITION - 1]);*/

	// ��ȡ�����ļ���Ϣ
    for (i = 0; i < INPUT_NUM_POSITION; i++)
	{
	    if (strcmp("input_file_width", szKeyWord[i]) == 0)
		{
		    (*sInputFileInfo).nOriWidth = atoi((*sInputFileInfo).szFilePath[INPUT_FILE_WIDTH_POSITION - 1]);
		}
		else if (strcmp("input_file_height", szKeyWord[i]) == 0)
		{
		    (*sInputFileInfo).nOriHeight = atoi((*sInputFileInfo).szFilePath[INPUT_FILE_HEIGHT_POSITION - 1]);
		}
		else if (strcmp("output_row", szKeyWord[i]) == 0)
		{
		    (*sInputFileInfo).nOutputRow = atoi((*sInputFileInfo).szFilePath[OUTPUT_ROW_POSITION - 1]);
		}
		else if (strcmp("output_col", szKeyWord[i]) == 0)
		{
		    (*sInputFileInfo).nOutputCol = atoi((*sInputFileInfo).szFilePath[OUTPUT_COL_POSITION - 1]);
		}
		else if (strcmp("input_file_num", szKeyWord[i]) == 0)
		{
		    (*sInputFileInfo).nInputNum = atoi((*sInputFileInfo).szFilePath[INPUT_NUM_POSITION - 1]);
		}
	}	
}

// ��ʼ���ļ���Ϣ
void initialize_file_struct(SOriginFileInfo* sOriginFileInfo, SResultFileInfo* sResultFileInfo, SInputFileInfo sInputFileInfo)
{
	// ԭYUV420pͼ��ߴ��С
	(*sOriginFileInfo).nOriWidth = sInputFileInfo.nOriWidth;
	(*sOriginFileInfo).nOriHeight = sInputFileInfo.nOriHeight;
	(*sOriginFileInfo).nOriSize = (*sOriginFileInfo).nOriWidth * (*sOriginFileInfo).nOriHeight * 3 / 2;

	// ԭYUV420pͼ��Y��U��V�����Ŀ��
	(*sOriginFileInfo).nOriYWidth = (*sOriginFileInfo).nOriWidth;
	(*sOriginFileInfo).nOriYHeight = (*sOriginFileInfo).nOriHeight;
	(*sOriginFileInfo).nOriUWidth = (*sOriginFileInfo).nOriYWidth / 2;
	(*sOriginFileInfo).nOriUHeight = (*sOriginFileInfo).nOriYHeight / 2;
	(*sOriginFileInfo).nOriVWidth = (*sOriginFileInfo).nOriUWidth;
	(*sOriginFileInfo).nOriVHeight = (*sOriginFileInfo).nOriUHeight;

	// U��V������ԭͼ���е������ʼλ��
	(*sOriginFileInfo).nOriUOffset = (*sOriginFileInfo).nOriYWidth * (*sOriginFileInfo).nOriYHeight;
	(*sOriginFileInfo).nOriVOffset = (*sOriginFileInfo).nOriUOffset + (*sOriginFileInfo).nOriUWidth * (*sOriginFileInfo).nOriUHeight;

	// �ϳɽ��YUV420pͼ��ߴ��С
	(*sResultFileInfo).nResWidth = sInputFileInfo.nOriWidth * sInputFileInfo.nOutputCol;
	(*sResultFileInfo).nResHeight = sInputFileInfo.nOriHeight * sInputFileInfo.nOutputRow;
	(*sResultFileInfo).nResSize = (*sResultFileInfo).nResWidth * (*sResultFileInfo).nResHeight * 3 / 2;

	// �ϳɽ��YUV420pͼ��Y��U��V�����Ŀ��
	(*sResultFileInfo).nResYWidth = (*sResultFileInfo).nResWidth;
	(*sResultFileInfo).nResYHeight = (*sResultFileInfo).nResHeight;
	(*sResultFileInfo).nResUWidth = (*sResultFileInfo).nResYWidth / 2;
	(*sResultFileInfo).nResUHeight = (*sResultFileInfo).nResYHeight / 2;
	(*sResultFileInfo).nResVWidth = (*sResultFileInfo).nResUWidth;
	(*sResultFileInfo).nResVHeight = (*sResultFileInfo).nResUHeight;

	// U��V�����ںϳɺ�ͼ���е������ʼλ��
	(*sResultFileInfo).nResUOffset = (*sResultFileInfo).nResYWidth * (*sResultFileInfo).nResYHeight;
	(*sResultFileInfo).nResVOffset = (*sResultFileInfo).nResUOffset + (*sResultFileInfo).nResUWidth * (*sResultFileInfo).nResUHeight;
}



