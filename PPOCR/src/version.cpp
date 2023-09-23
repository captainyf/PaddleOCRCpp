#include "version.h"
#include <stdio.h>

namespace PPOCR
{
	const char* GetVersion()
	{
		static char buf[128] = { 0 };
		int len = snprintf(buf, 128, "%s", PPOCR_VERSION);
		if (len < 0)
			return nullptr;
		else
			return buf;
	}
}