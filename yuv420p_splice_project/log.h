#ifndef _LOG_H
#define _LOG_H

// ��־�ļ���
const char* g_LogFileName;

// д��־
void write_log(const char* pLogFileName, const char* pErrorInfo, const char* pFileName, int nLineNum);


#endif