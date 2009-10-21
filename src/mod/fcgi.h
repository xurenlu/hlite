/*

    This file is Copyright (C) 2004  Craig Barkhouse

    This file is part of userver, a simple web server designed for
    performance experiments.

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License as
    published by the Free Software Foundation; either version 2 of the
    License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
    02111-1307 USA
*/

#ifndef FCGI_H
#define FCGI_H

#include <stdarg.h>

#include "fastcgi.h"
//#include "info.h"

typedef enum { GET, POST } REQUEST_METHOD;

#ifndef FCGI_ALIGN
#define FCGI_ALIGN 8
#endif

#define FCGI_PADDING_LEN(contentLength) \
  ((FCGI_ALIGN - ((contentLength) % FCGI_ALIGN)) % FCGI_ALIGN)

#define FCGI_HDR_TYPE(hdr) \
  (hdr)->type

#define FCGI_HDR_REQUESTID(hdr) \
  (((unsigned short) (hdr)->requestIdB1 << 8) + (hdr)->requestIdB0)

#define FCGI_HDR_CONTENT_LEN(hdr) \
  (((unsigned short) (hdr)->contentLengthB1 << 8) + (hdr)->contentLengthB0)

#define FCGI_HDR_PADDING_LEN(hdr) \
  (hdr)->paddingLength


int build_FCGI_hdr(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength);
int build_FCGI_padding(char *buf, int bufsize, unsigned short contentLength);
int build_FCGI_len(char *buf, int bufsize, int len);
int build_FCGI_namevalue_pair(char *buf, int bufsize, const char *name,
    const char *value);
int build_FCGI_request(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, unsigned short contentLength,
    const void *contentData);
int build_FCGI_request_str(char *buf, int bufsize, unsigned char type,
    unsigned short requestId, const char *contentStr);
int build_FCGI_BEGIN_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned short role, unsigned char flags);
int build_FCGI_ABORT_REQUEST(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_END_REQUEST(char *buf, int bufsize, unsigned short requestId,
    unsigned long appStatus, unsigned char protocolStatus);
int build_FCGI_PARAMS_list(char *buf, int bufsize, unsigned short requestId,
    va_list arglist);
int build_FCGI_PARAMS(char *buf, int bufsize, unsigned short requestId, ...);
int build_FCGI_END_PARAMS(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_STDIN(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDIN_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_END_STDIN(char *buf, int bufsize, unsigned short requestId);
int build_FCGI_STDOUT(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDOUT_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_STDERR(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_STDERR_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_DATA(char *buf, int bufsize, unsigned short requestId,
    unsigned short contentLength, const void *contentData);
int build_FCGI_DATA_str(char *buf, int bufsize, unsigned short requestId,
    const char *contentStr);
int build_FCGI_GET_VALUES_list(char *buf, int bufsize, va_list arglist);
int build_FCGI_GET_VALUES(char *buf, int bufsize, ...);
int build_FCGI_GET_VALUES_RESULT_list(char *buf, int bufsize, va_list arglist);
int build_FCGI_GET_VALUES_RESULT(char *buf, int bufsize, ...);
int build_FCGI_UNKNOWN_TYPE(char *buf, int bufsize, unsigned char type);

struct info *find_fcgi_idle(struct info *ip);
int fcgi_service_request(struct info *ip, int peer_sd);
int fcgi_service_next_request(struct info *ip);
int fcgi_process_request(int peer_sd);
int read_fcgi(struct info *ip);
int fcgi_add_to_reply_buffer_httpd(struct info *ip);

#define DO_FCGI_GET_SERVER_VALUES
#ifdef DO_FCGI_GET_SERVER_VALUES
int fcgi_get_server_values(struct info *ip);
#endif

#endif /* FCGI_H */
