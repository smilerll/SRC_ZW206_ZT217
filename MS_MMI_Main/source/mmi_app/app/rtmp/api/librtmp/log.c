#ifdef _DEBUG

#ifndef REAL_WATCH_RTOS
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#endif

#include "rtmp_sys.h"
#include "log.h"

#define MAX_PRINT_LEN  2048

static RTMP_LogLevel RTMP_debuglevel = RTMP_LOGDEBUG;

static const char *levels[] = {
    "CRIT", "ERROR", "WARNING", "INFO", "DEBUG", "DEBUG2"
};

static void rtmp_log_default(int level, const char *format, va_list vl)
{
    char str[MAX_PRINT_LEN] = "";
    vsnprintf(str, MAX_PRINT_LEN - 1, format, vl);
    /* Filter out 'no-name' */
    if (RTMP_debuglevel < RTMP_LOGALL && strstr(str, "no-name" ) != NULL)
        return;
    if (level <= RTMP_debuglevel)
    {
#ifdef REAL_WATCH_RTOS
   		SCI_TraceLow("RTMP %s: %s\n", levels[level], str);
#else
        fprintf(stderr, "%s: %s\n", levels[level], str);
        fflush(stderr);
#endif		
    }
}

void RTMP_Log(int level, const char *format, ...)
{
    va_list args;

    if (level > RTMP_debuglevel)
        return;

    va_start(args, format);
    rtmp_log_default(level, format, args);
    va_end(args);
}

static const char hexdig[] = "0123456789abcdef";

void RTMP_LogHex(int level, const uint8_t *data, unsigned long len)
{
    unsigned long i;
    char line[50], *ptr;

    if (level > RTMP_debuglevel)
        return;

    ptr = line;

    for (i = 0; i < len; i++)
    {
        *ptr++ = hexdig[0x0f & (data[i] >> 4)];
        *ptr++ = hexdig[0x0f & data[i]];
        if ((i & 0x0f) == 0x0f)
        {
            *ptr = '\0';
            ptr = line;
            RTMP_Log(level, "%s", line);
        } else
        {
            *ptr++ = ' ';
        }
    }
    if (i & 0x0f)
    {
        *ptr = '\0';
        RTMP_Log(level, "%s", line);
    }
}

void RTMP_LogHexString(int level, const uint8_t *data, unsigned long len)
{
#define BP_OFFSET 9
#define BP_GRAPH 60
#define BP_LEN   80
    char line[BP_LEN];
    unsigned long i;

    if (!data || level > RTMP_debuglevel)
        return;

    /* in case len is zero */
    line[0] = '\0';

    for (i = 0; i < len; i++)
    {
        int n = i % 16;
        unsigned off;

        if (!n)
        {
            if (i)
                RTMP_Log(level, "%s", line);
            memset(line, ' ', sizeof(line) - 2);
            line[sizeof(line) - 2] = '\0';

            off = i % 0x0ffffU;

            line[2] = hexdig[0x0f & (off >> 12)];
            line[3] = hexdig[0x0f & (off >>  8)];
            line[4] = hexdig[0x0f & (off >>  4)];
            line[5] = hexdig[0x0f & off];
            line[6] = ':';
        }

        off = BP_OFFSET + n*3 + ((n >= 8) ? 1 : 0);
        line[off]     = hexdig[0x0f & (data[i] >> 4)];
        line[off + 1] = hexdig[0x0f & data[i]];

        off = BP_GRAPH + n + ((n >= 8) ? 1 : 0);

        if (isprint(data[i]))
        {
            line[BP_GRAPH + n] = data[i];
        } else
        {
            line[BP_GRAPH + n] = '.';
        }
    }

    RTMP_Log(level, "%s", line);
}
#endif

/*
 *  Copyright (C) 2008-2009 Andrej Stepanchuk
 *  Copyright (C) 2009-2010 Howard Chu
 *
 *  This file is part of librtmp.
 *
 *  librtmp is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 2.1,
 *  or (at your option) any later version.
 *
 *  librtmp is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with librtmp see the file COPYING.  If not, write to
 *  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 *  http://www.gnu.org/copyleft/lgpl.html
 */
