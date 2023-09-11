#define PCH_H
// add headers that you want to pre-compile here
#include <winsock2.h>
#include <windows.h>

#include <cstdlib>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <ctime>
#include <ctime>
#include <string.h>
#include <string>
#include <vector>
#include <stdio.h>
#include <conio.h>
#include <array>
#include <map>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <tchar.h>
#include <inttypes.h>
#include <utility>
#include <corecrt_io.h>
#include <filesystem>
#include <codecvt>


#include "mem.h"
#include "util.h"

#include "Singleton.h"
#include "structs.h"
#include "PacketSend.h"
#include "MemReader.h"
#include "AutoAttack.h"

#pragma comment(lib, "winmm.lib")

#pragma comment(linker,"/manifestdependency:\"type='win32' "\
                   "name='Microsoft.Windows.Common-Controls' "\
                   "version='6.0.0.0' "\
                   "processorArchitecture='x86' "\
                   "publicKeyToken='6595b64144ccf1df' "\
                   "language='*' "\
                   "\"")


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define _WINSOCK_DEPRECATED_NO_WARNINGS 