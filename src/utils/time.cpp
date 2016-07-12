#include "time.h"

#include <Windows.h>

crlib::time::time()
{
	GetLocalTime(reinterpret_cast<LPSYSTEMTIME>(this));
}
