#include <stdio.h>
#include <string.h>
#include <time.h>

#include "log.h"

// Ð´ÈÕÖ¾
void write_log(const char* pLogFileName, const char* pErrorInfo, const char* pFileName, int nLineNum)
{
	FILE* fp = fopen(pLogFileName, "a");
	time_t tNow;
	char timeBuf[100];
	char* pFormat = "%Y-%m-%d %H:%M:%S";

	tNow = time((time_t*)0);

	strftime(timeBuf, sizeof(timeBuf), pFormat, gmtime(&tNow));

	fprintf(fp, "Date: %s\r\n", timeBuf);    // Ê¾Àý£º2015-10-07 20:46:01
	fprintf(fp, "Info: %s\r\n", pErrorInfo); 
	fprintf(fp, "File: %s\r\n", strrchr(pFileName, '\\') + 1);
	fprintf(fp, "Line: %d\r\n", nLineNum);

	fclose(fp);
}