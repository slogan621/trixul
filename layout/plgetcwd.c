/*
 *
 * Cross-platform getcwd 
 *
 * Copyright 2005 Syd Logan, All Rights Reserved
 *
 * This code is distributed without warranty. You are free to use this
 * code for any purpose, however, if this code is republished or
 * redistributed in its original form, as hardcopy or electronically,
 * then you must include this copyright notice along with the code.
 *
 */

#include "plgetcwd.h"

#if defined(XP_WIN)
#include <direct.h>
#else
#include <unistd.h>
#endif

#if defined(__cplusplus)
extern "C" {
#endif

char *
PL_getcwd(char *buf, size_t size)
{
#if defined(XP_WIN)
    return _getcwd(buf, size);
#else
    return getcwd(buf, size);
#endif
}

#if defined(__cplusplus)
}
#endif

