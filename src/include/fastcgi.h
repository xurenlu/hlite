/* 
 * fastcgi.h --
 *
 *	Defines for the FastCGI protocol.
 *
 *
 * Copyright (c) 1995-1996 Open Market, Inc.
 *
 * Quoted from LICENSE.TERMS:
 *
 * This FastCGI application library source and object code (the
 * "Software") and its documentation (the "Documentation") are
 * copyrighted by Open Market, Inc ("Open Market").  The following terms
 * apply to all files associated with the Software and Documentation
 * unless explicitly disclaimed in individual files.
 *
 * Open Market permits you to use, copy, modify, distribute, and license
 * this Software and the Documentation for any purpose, provided that
 * existing copyright notices are retained in all copies and that this
 * notice is included verbatim in any distributions.  No written
 * agreement, license, or royalty fee is required for any of the
 * authorized uses.  Modifications to this Software and Documentation may
 * be copyrighted by their authors and need not follow the licensing
 * terms described here.  If modifications to this Software and
 * Documentation have new licensing terms, the new terms must be clearly
 * indicated on the first page of each file where they apply.
 *
 * OPEN MARKET MAKES NO EXPRESS OR IMPLIED WARRANTY WITH RESPECT TO THE
 * SOFTWARE OR THE DOCUMENTATION, INCLUDING WITHOUT LIMITATION ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  IN
 * NO EVENT SHALL OPEN MARKET BE LIABLE TO YOU OR ANY THIRD PARTY FOR ANY
 * DAMAGES ARISING FROM OR RELATING TO THIS SOFTWARE OR THE
 * DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, ANY INDIRECT, SPECIAL OR
 * CONSEQUENTIAL DAMAGES OR SIMILAR DAMAGES, INCLUDING LOST PROFITS OR
 * LOST DATA, EVEN IF OPEN MARKET HAS BEEN ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGES.  THE SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS".
 * OPEN MARKET HAS NO LIABILITY IN CONTRACT, TORT, NEGLIGENCE OR
 * OTHERWISE ARISING OUT OF THIS SOFTWARE OR THE DOCUMENTATION.
 *
 * $Id: fastcgi.h,v 1.1 2004/08/18 17:43:54 cabarkho Exp $
 */

#ifndef _FASTCGI_H
#define _FASTCGI_H

/*
 * Listening socket file number
 */
#define FCGI_LISTENSOCK_FILENO 0

typedef struct {
    unsigned char version;
    unsigned char type;
    unsigned char requestIdB1;
    unsigned char requestIdB0;
    unsigned char contentLengthB1;
    unsigned char contentLengthB0;
    unsigned char paddingLength;
    unsigned char reserved;
} FCGI_Header;

#define FCGI_MAX_LENGTH 0xffff

/*
 * Number of bytes in a FCGI_Header.  Future versions of the protocol
 * will not reduce this number.
 */
#define FCGI_HEADER_LEN  8

/*
 * Value for version component of FCGI_Header
 */
#define FCGI_VERSION_1           1

/*
 * Values for type component of FCGI_Header
 */
#define FCGI_BEGIN_REQUEST       1
#define FCGI_ABORT_REQUEST       2
#define FCGI_END_REQUEST         3
#define FCGI_PARAMS              4
#define FCGI_STDIN               5
#define FCGI_STDOUT              6
#define FCGI_STDERR              7
#define FCGI_DATA                8
#define FCGI_GET_VALUES          9
#define FCGI_GET_VALUES_RESULT  10
#define FCGI_UNKNOWN_TYPE       11
#define FCGI_MAXTYPE (FCGI_UNKNOWN_TYPE)

/*
 * Value for requestId component of FCGI_Header
 */
#define FCGI_NULL_REQUEST_ID     0


typedef struct {
    unsigned char roleB1;
    unsigned char roleB0;
    unsigned char flags;
    unsigned char reserved[5];
} FCGI_BeginRequestBody;

typedef struct {
    FCGI_Header header;
    FCGI_BeginRequestBody body;
} FCGI_BeginRequestRecord;

/*
 * Mask for flags component of FCGI_BeginRequestBody
 */
#define FCGI_KEEP_CONN  1

/*
 * Values for role component of FCGI_BeginRequestBody
 */
#define FCGI_RESPONDER  1
#define FCGI_AUTHORIZER 2
#define FCGI_FILTER     3


typedef struct {
    unsigned char appStatusB3;
    unsigned char appStatusB2;
    unsigned char appStatusB1;
    unsigned char appStatusB0;
    unsigned char protocolStatus;
    unsigned char reserved[3];
} FCGI_EndRequestBody;

typedef struct {
    FCGI_Header header;
    FCGI_EndRequestBody body;
} FCGI_EndRequestRecord;

/*
 * Values for protocolStatus component of FCGI_EndRequestBody
 */
#define FCGI_REQUEST_COMPLETE 0
#define FCGI_CANT_MPX_CONN    1
#define FCGI_OVERLOADED       2
#define FCGI_UNKNOWN_ROLE     3


/*
 * Variable names for FCGI_GET_VALUES / FCGI_GET_VALUES_RESULT records
 */
#define FCGI_MAX_CONNS  "FCGI_MAX_CONNS"
#define FCGI_MAX_REQS   "FCGI_MAX_REQS"
#define FCGI_MPXS_CONNS "FCGI_MPXS_CONNS"


typedef struct {
    unsigned char type;    
    unsigned char reserved[7];
} FCGI_UnknownTypeBody;

typedef struct {
    FCGI_Header header;
    FCGI_UnknownTypeBody body;
} FCGI_UnknownTypeRecord;

#endif	/* _FASTCGI_H */

