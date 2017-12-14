#ifndef _SRC_STDAFX_H
#define _SRC_STDAFX_H

#include <stdio.h>
#include <stdarg.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <time.h>

#ifdef _WIN32
#include <tchar.h>
#include <Windows.h>
#include <winsock.h>
#elif __linux__
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <syslog.h>
#include <string.h>
#endif

#endif
