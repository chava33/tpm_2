/********************************************************************************/
/*										*/
/*			   Socket Transmit and Receive Utilities		*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*	      $Id: tstsocket.c 435 2015-11-20 15:15:41Z kgoldman $		*/
/*										*/
/* (c) Copyright IBM Corporation 2015.						*/
/*										*/
/* All rights reserved.								*/
/* 										*/
/* Redistribution and use in source and binary forms, with or without		*/
/* modification, are permitted provided that the following conditions are	*/
/* met:										*/
/* 										*/
/* Redistributions of source code must retain the above copyright notice,	*/
/* this list of conditions and the following disclaimer.			*/
/* 										*/
/* Redistributions in binary form must reproduce the above copyright		*/
/* notice, this list of conditions and the following disclaimer in the		*/
/* documentation and/or other materials provided with the distribution.		*/
/* 										*/
/* Neither the names of the IBM Corporation nor the names of its		*/
/* contributors may be used to endorse or promote products derived from		*/
/* this software without specific prior written permission.			*/
/* 										*/
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS		*/
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT		*/
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR	*/
/* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT		*/
/* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,	*/
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT		*/
/* LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,	*/
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY	*/
/* THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT		*/
/* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE	*/
/* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.		*/
/********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>

/* TST_SOCKET_FD encapsulates the differences between the Posix and Windows socket type */

#ifdef TPM_POSIX
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

/* typedef int TST_SOCKET_FD; */
#endif
#ifdef TPM_WINDOWS
#include <winsock2.h>
/* typedef SOCKET TST_SOCKET_FD;  */
#endif

#include <sys/types.h>
#include <fcntl.h>

#include "Unmarshal_fp.h"
#include "TpmTcpProtocol.h"	/* for MS simulator admin commands */
#include "tseresponsecode.h"
#include "tseerror.h"
#include "tseprint.h"
#include "tseproperties.h"

#include "tstsocket.h"

/* local prototypes */

static uint32_t TST_Socket_Open(TST_SOCKET_FD *sock_fd, short port);
static uint32_t TST_Socket_SendCommand(TST_SOCKET_FD sock_fd, const uint8_t *buffer, uint16_t length,
				       const char *message);
static uint32_t TST_Socket_SendPlatform(TST_SOCKET_FD sock_fd, uint32_t command, const char *message);
static uint32_t TST_Socket_ReceiveCommand(TST_SOCKET_FD sock_fd, uint8_t *buffer, uint32_t *length);
static uint32_t TST_Socket_ReceivePlatform(TST_SOCKET_FD sock_fd);
static uint32_t TST_Socket_Close(TST_SOCKET_FD sock_fd);
static uint32_t TST_Socket_ReceiveBytes(TST_SOCKET_FD sock_fd, uint8_t *buffer, uint32_t nbytes);
static uint32_t TST_Socket_SendBytes(TST_SOCKET_FD sock_fd, const uint8_t *buffer, size_t length);

static uint32_t TST_Socket_GetServerType(int *mssim);

extern BOOL tseVverbose;
extern BOOL tseVerbose;
extern TSE_CONTEXT tseContext;

/* TST_Socket_TransmitPlatform() transmits MS simulator platform administrative commands */

TPM_RC TST_Socket_TransmitPlatform(uint32_t command, const char *message)
{
    TPM_RC 	rc = 0;
    int 	mssim;	/* boolean, true for MS simulator packet format, false for raw packet
			   format */

    /* detect errors before starting, get the server packet type, MS sim or raw */
    if (rc == 0) {
	rc = TST_Socket_GetServerType(&mssim);
    }
    /* the platform administrative commands can only work with the simulator */
    if (rc == 0) {
	if (!mssim) {
	    if (tseVerbose) printf("TST_Socket_TransmitPlatform: server type %s unsupported\n",
				   tseContext.tseServerType);
	    rc = TSE_RC_INSUPPORTED_INTERFACE;	
	}
    }
    if (rc == 0) {
	rc = TST_Socket_Open(&tseContext.sock_fd, tseContext.tsePlatformPort);
    }
    if (rc == 0) {
	rc = TST_Socket_SendPlatform(tseContext.sock_fd, command, message);
    }
    if (rc == 0) {
	rc = TST_Socket_ReceivePlatform(tseContext.sock_fd);
    }
    if (rc == 0) {
	rc = TST_Socket_Close(tseContext.sock_fd);
    }
    return rc;
}

/* TST_Socket_Transmit() transmits the TPM command and receives the response.

   It can return socket transmit and receive packet errors, but normally returns the TPM response
   code.

*/

TPM_RC TST_Socket_Transmit(uint8_t *responseBuffer, uint32_t *read,
			   const uint8_t *commandBuffer, uint32_t written,
			   const char *message)
{
    TPM_RC 	rc = 0;
    int 	mssim;	/* boolean, true for MS simulator packet format, false for raw packet
			   format */

    /* open on first transmit */
    if (tseContext.tseFirstTransmit) {	
	/* detect errors before starting, get the server packet type, MS sim or raw */
	if (rc == 0) {
	    rc = TST_Socket_GetServerType(&mssim);
	}
	if (rc == 0) {
	    rc = TST_Socket_Open(&tseContext.sock_fd, tseContext.tseCommandPort);
	}
	tseContext.tseFirstTransmit = FALSE;
    }
    /* send the command over the socket.  Error if the socket send fails. */
    if (rc == 0) {
	/* opened = TRUE; */
	rc = TST_Socket_SendCommand(tseContext.sock_fd, commandBuffer, written, message);
    }
    /* receive the response over the socket.  Returns socket errors, malformed response errors.
       Else returns the TPM response code. */
    if (rc == 0) {
	rc = TST_Socket_ReceiveCommand(tseContext.sock_fd, responseBuffer, read);
    }
#if 0
    /* close even on error */
    if (opened) {
	rc1 = TST_Socket_Close(tseContext.sock_fd);
	/* receive error takes precedence over close error */
	if (rc == 0) {
	    rc = rc1;
	}
    }
#endif
    return rc;
}

/* TST_Socket_GetServerType() gets the type of server packet format

   Currently, the two formats supported are:

   mssim

   TRUE  - the MS simulator packet
   FALSE - raw TPM specification Part 3 packets
*/

static uint32_t TST_Socket_GetServerType(int *mssim)
{
    uint32_t 	rc = 0;
    if (rc == 0) {
	if ((strcmp(tseContext.tseServerType, "mssim") == 0)) {
	    *mssim = TRUE;
	}
	else if ((strcmp(tseContext.tseServerType, "raw") == 0)) {
	    *mssim = FALSE;
	}
	else {
	    if (tseVerbose) printf("TST_Socket_GetServerType: server type %s unsupported\n",
				   tseContext.tseServerType);
	    rc = TSE_RC_INSUPPORTED_INTERFACE;	
	}
    }
    return rc;
}

/* TST_Socket_Open() opens the socket to the TPM Host emulation to tseServerName:port

*/

static uint32_t TST_Socket_Open(TST_SOCKET_FD *sock_fd, short port)
{
#ifdef TPM_WINDOWS 
    WSADATA 		wsaData;
    int			irc;
#endif
    struct sockaddr_in 	serv_addr;
    struct hostent 	*host = NULL;

    if (tseVverbose) printf("TST_Socket_Open: Opening %s:%hu-%s\n",
			    tseContext.tseServerName, port, tseContext.tseServerType);
    /* create a socket */
#ifdef TPM_WINDOWS
    if ((irc = WSAStartup(0x202, &wsaData)) != 0) {		/* if not successful */
	if (tseVerbose) printf("TST_Socket_Open: Error, WSAStartup failed\n");
	WSACleanup();
	return TSE_RC_NO_CONNECTION;
    }
    if ((*sock_fd = socket(AF_INET,SOCK_STREAM, 0)) == INVALID_SOCKET) {
	if (tseVerbose) printf("TST_Socket_Open: client socket() error: %u\n", *sock_fd);
	return TSE_RC_NO_CONNECTION;
    }
#endif 
#ifdef TPM_POSIX
    if ((*sock_fd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
	if (tseVerbose) printf("TST_Socket_Open: client socket error: %d %s\n",
			       errno,strerror(errno));
	return TSE_RC_NO_CONNECTION;
    }
#endif
    memset((char *)&serv_addr,0x0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    /* the server host name tseServerName came from the default or an environment variable */
    /* first assume server is dotted decimal number and call inet_addr */
    if ((int)(serv_addr.sin_addr.s_addr = inet_addr(tseContext.tseServerName)) == -1) {
	/* if inet_addr fails, assume server is a name and call gethostbyname to look it up */
	if ((host = gethostbyname(tseContext.tseServerName)) == NULL) {	/* if gethostbyname also fails */
	    if (tseVerbose) printf("TST_Socket_Open: server name error, name %s\n",
				   tseContext.tseServerName);
	    return TSE_RC_NO_CONNECTION;
	}
	serv_addr.sin_family = host->h_addrtype;
	memcpy(&serv_addr.sin_addr, host->h_addr, host->h_length);
    }
    /* establish the connection to the TPM server */
#ifdef TPM_POSIX
    if (connect(*sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
	if (tseVerbose) printf("TST_Socket_Open: Error on connect to %s:%u\n",
			       tseContext.tseServerName, port);
	if (tseVerbose) printf("TST_Socket_Open: client connect: error %d %s\n",
			       errno,strerror(errno));
	return TSE_RC_NO_CONNECTION;
    }
#endif
#ifdef TPM_WINDOWS
    if (connect(*sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) != 0) {
	if (tseVerbose) printf("TST_Socket_Open: Error on connect to %s:%u\n",
			       tseContext.tseServerName, port);
	if (tseVerbose) printf("TST_Socket_Open: client connect: error %d %s\n",
			       errno,strerror(errno));
	return TSE_RC_NO_CONNECTION;
    }
#endif
    else {
	/*  	printf("TST_Socket_Open: client connect: success\n"); */
    }
    return 0;
}

/* TST_Socket_SendCommand() sends the TPM command packet over the socket.

   The MS simulator packet is of the form:

   TPM_SEND_COMMAND
   locality 0
   length
   TPM command packet	(this is the raw packet format)

   Returns an error if the socket send fails.
*/

static uint32_t TST_Socket_SendCommand(TST_SOCKET_FD sock_fd,
				       const uint8_t *buffer, uint16_t length,
				       const char *message)
{
    uint32_t 	rc = 0;
    int 	mssim;	/* boolean, true for MS simulator packet format, false for raw packet
			   format */
    
    if (message != NULL) {
	if (tseVverbose) printf("TST_Socket_SendCommand: %s\n", message);
    }
    /* trace the command packet */
    if ((rc == 0) && tseVverbose) {
	TPM_PrintAll("TST_Socket_SendCommand",
		     buffer, length);
    }
    /* get the server packet type, MS sim or raw */
    if (rc == 0) {
	rc = TST_Socket_GetServerType(&mssim);
    }
    /* MS simulator wants a command type, locality, length */
    if ((rc == 0) && mssim) {
	uint32_t commandType = htonl(TPM_SEND_COMMAND);	/* command type is network byte order */
	rc = TST_Socket_SendBytes(sock_fd, (uint8_t *)&commandType, sizeof(uint32_t));
    }
    if ((rc == 0) && mssim) {
	uint8_t locality = 0;
	rc = TST_Socket_SendBytes(sock_fd, &locality, sizeof(uint8_t));
    }
    if ((rc == 0) && mssim) {
	uint32_t lengthNbo = htonl(length);	/* length is network byte order */
	rc = TST_Socket_SendBytes(sock_fd, (uint8_t *)&lengthNbo, sizeof(uint32_t));
    }
    /* all packet formats (types) send the TPM command packet */
    if (rc == 0) {
	rc = TST_Socket_SendBytes(sock_fd, buffer, length);
    }
    return rc;
}

/* TST_Socket_SendPlatform() transmits MS simulator platform administrative commands.  This function
   should only be called if the TPM supports administrative commands.

   Returns an error if the socket send fails.

*/

static uint32_t TST_Socket_SendPlatform(TST_SOCKET_FD sock_fd, uint32_t command, const char *message)
{
    uint32_t rc = 0;

    if (message != NULL) {
	if (tseVverbose) printf("TST_Socket_SendPlatform: %s\n", message);
    }
    /* MS simulator platform commands */
    if (rc == 0) {
	uint32_t commandNbo = htonl(command);	/* command is network byte order */
	rc = TST_Socket_SendBytes(sock_fd, (uint8_t *)&commandNbo , sizeof(uint32_t));
    }
    return rc;
}

/* TST_Socket_SendBytes() is the low level sent function that transmits the buffer over the socket.

   It handles partial writes by looping.

 */

static uint32_t TST_Socket_SendBytes(TST_SOCKET_FD sock_fd, const uint8_t *buffer, size_t length)
{
    int nwritten = 0;
    size_t nleft = 0;
    unsigned int offset = 0;

    nleft = length;
    while (nleft > 0) {
#ifdef TPM_POSIX
	nwritten = write(sock_fd, &buffer[offset], nleft);
	if (nwritten < 0) {        /* error */
	    if (tseVerbose) printf("TST_Socket_SendBytes: write error %d\n", (int)nwritten);
	    return TSE_RC_BAD_CONNECTION;
	}
#endif
#ifdef TPM_WINDOWS
	/* cast for winsock.  Unix uses void * */
	nwritten = send(sock_fd, (char *)(&buffer[offset]), nleft, 0);
	if (nwritten == SOCKET_ERROR) {        /* error */
	    if (tseVerbose) printf("TST_Socket_SendBytes: write error %d\n", (int)nwritten);
	    return TSE_RC_BAD_CONNECTION;
	}
#endif
	nleft -= nwritten;
	offset += nwritten;
    }
    return 0;
}

/* TST_Socket_ReceiveCommand() reads a TPM response packet from the socket.  'buffer' must be at
   least MAX_RESPONSE_SIZE bytes.  The bytes read are returned in 'length'.

   The MS simulator packet is of the form:

   length
   TPM response packet		(this is the raw packet format)
   acknowledgement uint32_t zero

   If the receive succeeds, returns TPM packet error code.

   Validates that the packet length and the packet responseSize match 
*/

static uint32_t TST_Socket_ReceiveCommand(TST_SOCKET_FD sock_fd, uint8_t *buffer, uint32_t *length)
{
    uint32_t 	rc = 0;
    uint32_t 	responseSize = 0;
    uint32_t 	responseLength = 0;
    uint8_t 	*bufferPtr = buffer;	/* the moving buffer */
    TPM_RC 	responseCode;
    INT32 	size;		/* dummy for unmarshal call */
    int 	mssim;	/* boolean, true for MS simulator packet format, false for raw packet
			   format */
    
    /* get the server packet type, MS sim or raw */
    if (rc == 0) {
	rc = TST_Socket_GetServerType(&mssim);
    }
    /* read the length prepended by the simulator */
    if ((rc == 0) && mssim) {
	rc = TST_Socket_ReceiveBytes(sock_fd, (uint8_t *)&responseLength, sizeof(uint32_t));
	responseLength = ntohl(responseLength);
    }
    /* read the tag and responseSize */
    if (rc == 0) {
	rc = TST_Socket_ReceiveBytes(sock_fd, bufferPtr, sizeof(TPM_ST) + sizeof(uint32_t));
    }
    /* extract the responseSize */
    if (rc == 0) {
	/* skip over tag to responseSize */
	bufferPtr += sizeof(TPM_ST);
	
	size = sizeof(uint32_t);		/* dummy for call */
	rc = UINT32_Unmarshal(&responseSize, &bufferPtr, &size);
	*length = responseSize;			/* returned length */

	/* check the response size, see TSS_CONTEXT structure */
	if (responseSize > MAX_RESPONSE_SIZE) {
	    if (tseVerbose)
		printf("TST_Socket_ReceiveCommand: ERROR: responseSize %u greater than %u\n",
		       responseSize, MAX_RESPONSE_SIZE);
	    rc = TSE_RC_BAD_CONNECTION;
	}
	/* check that MS sim prepended length is the same as the response TPM packet
	   length parameter */
	if (mssim && (responseSize != responseLength)) {
	    if (tseVerbose) printf("TST_Socket_ReceiveCommand: "
				   "ERROR: responseSize %u not equal to responseLength %u\n",
				   responseSize, responseLength);
	    rc = TSE_RC_BAD_CONNECTION;
	}
    }
    /* read the rest of the packet */
    if (rc == 0) {
	rc = TST_Socket_ReceiveBytes(sock_fd,
				     bufferPtr,
				     responseSize - (sizeof(TPM_ST) + sizeof(uint32_t)));
    }
    if ((rc == 0) && tseVverbose) {
	TPM_PrintAll("TST_Socket_ReceiveCommand",
		     buffer, responseSize);
    }
    /* extract the TPM return code from the packet */
    if (rc == 0) {
	/* skip to responseCode */
	bufferPtr = buffer + sizeof(TPM_ST) + sizeof(uint32_t);
	size = sizeof(TPM_RC);		/* dummy for call */
	rc = UINT32_Unmarshal(&responseCode, &bufferPtr, &size);
    }
    /* if there is no other error, return the TPM response code */
    if (rc == 0) {
	rc = responseCode;
    }
    /* read the MS sim acknowledgement */
    TPM_RC 	acknowledgement;
    if ((rc == 0) && mssim) {
	rc = TST_Socket_ReceiveBytes(sock_fd, (uint8_t *)&acknowledgement, sizeof(uint32_t));
    }
    /* if there is no other error, return the MS simulator packet acknowledgement */
    if ((rc == 0) && mssim) {
	  rc = ntohl(acknowledgement);	/* should always be zero */
    }
    return rc;
}

/* TST_Socket_ReceivePlatform reads MS simulator platform administrative responses.  This function
   should only be called if the TPM supports administrative commands.

   The acknowledgement is a uint32_t zero.

*/

static uint32_t TST_Socket_ReceivePlatform(TST_SOCKET_FD sock_fd)
{
    uint32_t 	rc = 0;
    TPM_RC 	acknowledgement;
    
    /* read the MS sim acknowledgement */
    if (rc == 0) {
	rc = TST_Socket_ReceiveBytes(sock_fd, (uint8_t *)&acknowledgement, sizeof(uint32_t));
    }
    /* if there is no other error, return the MS simulator packet acknowledgement */
    if (rc == 0) {
	rc = ntohl(acknowledgement);	/* should always be zero */
    }
    return rc;
}

/* TST_Socket_ReceiveBytes() is the low level receive function that reads the buffer over the
   socket.  'buffer' must be atleast 'nbytes'. 

   It handles partial reads by looping.

*/

static uint32_t TST_Socket_ReceiveBytes(TST_SOCKET_FD sock_fd,
					uint8_t *buffer,  
					uint32_t nbytes)
{
    int nread = 0;
    int nleft = 0;

    nleft = nbytes;
    while (nleft > 0) {
#ifdef TPM_POSIX
	nread = read(sock_fd, buffer, nleft);
	if (nread <= 0) {       /* error */
	    if (tseVerbose)  printf("TST_Socket_ReceiveBytes: read error %d\n", nread);
	    return TSE_RC_BAD_CONNECTION;
	}
#endif
#ifdef TPM_WINDOWS
	/* cast for winsock.  Unix uses void * */
	nread = recv(sock_fd, (char *)buffer, nleft, 0);
	if (nread == SOCKET_ERROR) {       /* error */
	    if (tseVerbose) printf("TST_Socket_ReceiveBytes: read error %d\n", nread);
	    return TSE_RC_BAD_CONNECTION;
	}
#endif
	else if (nread == 0) {  /* EOF */
	    if (tseVerbose) printf("TST_Socket_ReceiveBytes: read EOF\n");
	    return TSE_RC_BAD_CONNECTION;
	}
	nleft -= nread;
	buffer += nread;
    }
    return 0;
}

/* TST_Socket_Close() closes the socket.

   It sends the TPM_SESSION_END required by the MS simulator.

*/

static uint32_t TST_Socket_Close(TST_SOCKET_FD sock_fd)
{
    uint32_t 	rc = 0;
    int 	mssim;	/* boolean, true for MS simulator packet format, false for raw packet
			   format */
    
    /* get the server packet type, MS sim or raw */
    if (rc == 0) {
	rc = TST_Socket_GetServerType(&mssim);
    }

    /* the MS simulator expects a TPM_SESSION_END command before close */
    if ((rc == 0) && mssim) {
	uint32_t commandType = htonl(TPM_SESSION_END);
	rc = TST_Socket_SendBytes(sock_fd, (uint8_t *)&commandType, sizeof(uint32_t));
    }
#ifdef TPM_POSIX
    if (close(sock_fd) != 0) {
	if (tseVerbose) printf("TST_Socket_Close: close error\n");
	rc = TSE_RC_BAD_CONNECTION;
    }
#endif
#ifdef TPM_WINDOWS
    /* gracefully shut down the socket */
    if (rc == 0) {
	int		irc;
	irc = shutdown(sock_fd, SD_SEND);
	if (irc == SOCKET_ERROR) {       /* error */
	    if (tseVerbose) printf("TST_Socket_Close: shutdown error\n");
	    rc = TSE_RC_BAD_CONNECTION;
	}
    }
    closesocket(sock_fd);
    WSACleanup();
#endif
    return rc;
}
