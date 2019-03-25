#ifndef _CONFIG_H
#define _CONFIG_H

// ��������ļ�·�����������ļ����������������ļ�����
#define OUTPUT_POSITION 1
// ���������ļ��г���������־λ��
#define PROCEDURE_ON_POSITION 1
// ���������ļ�������ͼƬ����λ��
#define INPUT_FILE_WIDTH_POSITION 2
#define INPUT_FILE_HEIGHT_POSITION 3
// ���������ļ�������ļ��С���λ��
#define OUTPUT_ROW_POSITION 4
#define OUTPUT_COL_POSITION 5
// �����ļ������������ļ���Ϊ�����У��̶������޸�
#define INPUT_NUM_POSITION 6

// ���������ļ���Ϣ�ṹ��
typedef struct SInputFileInfo
{
    int nOriWidth;      // �����ļ���
	int nOriHeight;     // �����ļ���
	int nOutputRow;     // ����ļ�����
	int nOutputCol;     // ����ļ�����
	int nInputNum;      // �����ļ���
	char szFilePath[50][256];    // �ļ�·������
} SInputFileInfo;

// ����ԭYUV420p�ļ��ߴ���Ϣ
typedef struct SOriginFileInfo
{
    // ԭYUV420pͼ��ߴ��С
	int nOriWidth;
	int nOriHeight;
	int nOriSize;

	// ԭYUV420pͼ��Y��U��V�����Ŀ��
	int nOriYWidth;
	int nOriYHeight;
	int nOriUWidth;
	int nOriUHeight;
	int nOriVWidth;
	int nOriVHeight;

	// U��V������ԭͼ���е������ʼλ��
	int nOriUOffset;
	int nOriVOffset;
} SOriginFileInfo;

// ����ϳɽ��YUV420p�ļ��ߴ���Ϣ
typedef struct SResultFileInfo
{
    // �ϳɽ��YUV420pͼ��ߴ��С
	int nResWidth;
	int nResHeight;
	int nResSize;

	// �ϳɽ��YUV420pͼ��Y��U��V�����Ŀ��
	int nResYWidth;
	int nResYHeight;
	int nResUWidth;
	int nResUHeight;
	int nResVWidth;
	int nResVHeight;

	// U��V�����ںϳɺ�ͼ���е������ʼλ��
	int nResUOffset;
	int nResVOffset;
} SResultFileInfo;

// ���������ļ�
void load_config(const char* pFilePath, SInputFileInfo* sInputFileInfo);
// ��ʼ���ļ���Ϣ
void initialize_file_struct(SOriginFileInfo* sOriginFileInfo, SResultFileInfo* sResultFileInfo, SInputFileInfo sInputFileInfo);



#endif