#ifndef _LOG_H
#define _LOG_H

// 日志文件名
const char* g_LogFileName;

// 写日志
void write_log(const char* pLogFileName, const char* pErrorInfo, const char* pFileName, int nLineNum);


#endif