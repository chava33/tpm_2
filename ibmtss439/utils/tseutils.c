/********************************************************************************/
/*										*/
/*			    TSS, TSE and Application Utilities			*/
/*			     Written by Ken Goldman				*/
/*		       IBM Thomas J. Watson Research Center			*/
/*		$Id: tseutils.c 430 2015-11-19 16:24:33Z kgoldman $		*/
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
#include <errno.h>

#ifdef TPM_POSIX
#include <netinet/in.h>
#endif
#ifdef TPM_WINDOWS
#include <winsock2.h>
#endif

#include "tseutils.h"
#include "tseresponsecode.h"
#include "tseerror.h"

#define TSE_ALLOC_MAX  0x10000  /* 64k bytes */

extern BOOL tseVerbose;

/* TSE_Malloc() is a general purpose wrapper around malloc()
 */

TPM_RC TSE_Malloc(unsigned char **buffer, uint32_t size)
{
    TPM_RC          rc = 0;
    
    /* assertion test.  The coding style requires that all allocated pointers are initialized to
       NULL.  A non-NULL value indicates either a missing initialization or a pointer reuse (a
       memory leak). */
    if (rc == 0) {
        if (*buffer != NULL) {
            if (tseVerbose)
		printf("TSE_Malloc: Error (fatal), *buffer %p should be NULL before malloc\n", *buffer);
            rc = TSE_RC_ALLOC_INPUT;
        }
    }
    /* verify that the size is not "too large" */
    if (rc == 0) {
        if (size > TSE_ALLOC_MAX) {
            if (tseVerbose) printf("TSE_Malloc: Error, size %u greater than maximum allowed\n", size);
            rc = TSE_RC_MALLOC_SIZE;
        }       
    }
    /* verify that the size is not 0, this would be implementation defined and should never occur */
    if (rc == 0) {
        if (size == 0) {
            if (tseVerbose) printf("TSE_Malloc: Error (fatal), size is zero\n");
            rc = TSE_RC_MALLOC_SIZE;
        }       
    }
    if (rc == 0) {
        *buffer = malloc(size);
        if (*buffer == NULL) {
            if (tseVerbose) printf("TSE_Malloc: Error allocating %u bytes\n", size);
            rc = TSE_RC_OUT_OF_MEMORY;
        }
    }
    return rc;
}

/* File_Open() opens the 'filename' for 'mode'
 */

int File_Open(FILE **file,
	      const char *filename,
	      const char* mode)
{
    int 	rc = 0;
		    
    if (rc == 0) {
	*file = fopen(filename, mode);
	if (*file == NULL) {
	    if (tseVerbose) printf("File_Open: Error opening %s for %s, %s\n",
				   filename, mode, strerror(errno));
	    rc = TSE_RC_FILE_OPEN;
	}
    }
    return rc;
}

/* File_ReadBinaryFile() reads 'filename'.  The results are put into 'data', which must be freed by
   the caller.  'length' indicates the number of bytes read. 'length_max' is the maximum allowed
   length.

*/

TPM_RC File_ReadBinaryFile(unsigned char **data,     /* must be freed by caller */
			   size_t *length,
			   const char *filename) 
{
    int		rc = 0;
    long	lrc;
    size_t	src;
    int		irc;
    FILE	*file = NULL;

    *data = NULL;
    *length = 0;
    /* open the file */
    if (rc == 0) {
	rc = File_Open(&file, filename, "rb");				/* closed @1 */
    }
    /* determine the file length */
    if (rc == 0) {
	irc = fseek(file, 0L, SEEK_END);	/* seek to end of file */
	if (irc == -1L) {
	    if (tseVerbose) printf("File_ReadBinaryFile: Error seeking to end of %s\n", filename);
	    rc = TSE_RC_FILE_SEEK;
	}
    }
    if (rc == 0) {
	lrc = ftell(file);			/* get position in the stream */
	if (lrc == -1L) {
	    if (tseVerbose) printf("File_ReadBinaryFile: Error ftell'ing %s\n", filename);
	    rc = TSE_RC_FILE_FTELL;
	}
	else {
	    *length = (size_t)lrc;		/* save the length */
	}
    }
    if (rc == 0) {
	irc = fseek(file, 0L, SEEK_SET);	/* seek back to the beginning of the file */
	if (irc == -1L) {
	    if (tseVerbose) printf("File_ReadBinaryFile: Error seeking to beginning of %s\n",
				   filename);
	    rc = TSE_RC_FILE_SEEK;
	}
    }
    /* allocate a buffer for the actual data */
    if ((rc == 0) && (*length != 0)) {
	rc = TSE_Malloc(data, *length);
    }
    /* read the contents of the file into the data buffer */
    if ((rc == 0) && *length != 0) {
	src = fread(*data, 1, *length, file);
	if (src != *length) {
	    if (tseVerbose) printf("File_ReadBinaryFile: Error reading %s, %u bytes\n",
				   filename, (unsigned int)*length);
	    rc = TSE_RC_FILE_READ;
	}
    }
    if (file != NULL) {
	irc = fclose(file);		/* @1 */
	if (irc != 0) {
	    if (tseVerbose) printf("File_ReadBinaryFile: Error closing %s\n",
				   filename);
	    rc = TSE_RC_FILE_CLOSE;
	}
    }
    if (rc != 0) {
	if (tseVerbose) printf("File_ReadBinaryFile: Error reading %s\n", filename);
	free(*data);
	data = NULL;
    }
    return rc;
}

TPM_RC File_WriteBinaryFile(const unsigned char *data,
			    size_t length,
			    const char *filename) 
{
    long	rc = 0;
    size_t	src;
    int		irc;
    FILE	*file = NULL;

    /* open the file */
    if (rc == 0) {
	rc = File_Open(&file, filename, "wb");	/* closed @1 */
    }
    /* write the contents of the data buffer into the file */
    if (rc == 0) {
	src = fwrite(data, 1, length, file);
	if (src != length) {
	    if (tseVerbose) printf("File_WriteBinaryFile: Error writing %s\n",
				   filename);
	    rc = TSE_RC_FILE_WRITE;
	}
    }
    if (file != NULL) {
	irc = fclose(file);		/* @1 */
	if (irc != 0) {
	    if (tseVerbose) printf("File_WriteBinaryFile: Error closing %s\n",
				   filename);
	    rc = TSE_RC_FILE_CLOSE;
	}
    }
    return rc;
}

/* File_ReadStructure() is a general purpose "read a structure" function.

   It reads the filename, and then unmarshals the structure using "unmarshalFunction".
*/

TPM_RC File_ReadStructure(void 			*structure,
			  UnmarshalFunction_t 	unmarshalFunction,
			  const char 		*filename)
{
    TPM_RC 	rc = 0;
    uint8_t	*buffer = NULL;		/* for the free */
    uint8_t	*buffer1 = NULL;	/* for unmarshaling */
    size_t 	length = 0;

    if (rc == 0) {
	rc = File_ReadBinaryFile(&buffer,     /* must be freed by caller */
				 &length,
				 filename);
    }
    if (rc == 0) {
	buffer1 = buffer;
	rc = unmarshalFunction(structure, &buffer1, (int32_t *)&length);
    }
    free(buffer);
    return rc;
}

/* File_WriteStructure() is a general purpose "write a structure" function.

   It marshals the structure using "marshalFunction", and then writes it to filename.
*/

TPM_RC File_WriteStructure(void 		*structure,
			   MarshalFunction_t 	marshalFunction,
			   const char 		*filename)
{
    TPM_RC 	rc = 0;
    uint16_t	written = 0;;
    uint8_t	*buffer = NULL;		/* for the free */
    uint8_t	*buffer1 = NULL;	/* for marshaling */

    /* marshal once to calculates the byte length */
    if (rc == 0) {
	rc = marshalFunction(structure, &written, NULL, NULL);
    }
     if (rc == 0) {
	 rc = TSE_Malloc(&buffer, written);
     }
     if (rc == 0) {
	buffer1 = buffer;
	written = 0;
	rc = marshalFunction(structure, &written, &buffer1, NULL);
    }
    if (rc == 0) {
	rc = File_WriteBinaryFile(buffer,
				  written,
				  filename); 
    }
    free(buffer);
    return rc;
}

TPM_RC File_Read2B(TPM2B 	*tpm2b,
		   uint16_t 	targetSize,
		   const char 	*filename)
{
    TPM_RC 	rc = 0;
    uint8_t	*buffer = NULL;
    size_t 	length = 0;
    
    if (rc == 0) {
	rc = File_ReadBinaryFile(&buffer,     /* must be freed by caller */
				 &length,
				 filename);
    }
    /* copy it into the TPM2B */
    if (rc == 0) {
	rc = TPM2B_Create(tpm2b, buffer, length, targetSize);
    }
    free(buffer);
    return rc;
}

/* FIXME need to add - ignore failure if does not exist */

TPM_RC File_DeleteFile(const char *filename) 
{
    TPM_RC 	rc = 0;
    int		irc;
    
    if (rc == 0) {
	irc = remove(filename);
	if (irc != 0) {
	    rc = TSE_RC_FILE_REMOVE;
	}
    }
    return rc;
}

/* TPM2B_Copy() copies source to target if the source fits the target size */

TPM_RC TPM2B_Copy(TPM2B *target, TPM2B *source, uint16_t targetSize)
{
    TPM_RC rc = 0;

    if (rc == 0) {
	if (source->size > targetSize) {
	    if (tseVerbose) printf("TPM2B_Copy: size %u greater than target %u\n",
				   source->size, targetSize);	
	    rc = TSE_RC_INSUFFICIENT_BUFFER;
	}
    }
    if (rc == 0) {
	memmove(target->buffer, source->buffer, source->size);
	target->size = source->size;
    }
    return rc;
}

/* TPM2B_Append() appends the source TPM2B to the target TPM2B.

   It checks that the source fits the target size. The target size is the total size, not the size
   remaining.
*/

TPM_RC TPM2B_Append(TPM2B *target, TPM2B *source, uint16_t targetSize)
{
    TPM_RC rc = 0;

    if (rc == 0) {
	if (target->size + source->size > targetSize) {
	    if (tseVerbose) printf("TPM2B_Append: size %u greater than target %u\n",
				   target->size + source->size, targetSize);	
	    rc = TSE_RC_INSUFFICIENT_BUFFER;
	}
    }
    if (rc == 0) {
	memmove(target->buffer + target->size, source->buffer, source->size);
	target->size += source->size;
    }
    return rc;
}

/* TPM2B_Create() copies the buffer of 'size' into target, checking targetSize */

TPM_RC TPM2B_Create(TPM2B *target, uint8_t *buffer, uint16_t size, uint16_t targetSize)
{
    TPM_RC rc = 0;
    
    if (rc == 0) {
	if (size > targetSize) {
	    if (tseVerbose) printf("TPM2B_Create: size %u greater than target %u\n",
				   size, targetSize);	
	    rc = TSE_RC_INSUFFICIENT_BUFFER;
	}
    }
    if (rc == 0) {
	target->size = size;
	memmove(target->buffer, buffer, size);
    }
    return rc;
}

/* TPM2B_CreateUint32() creates a TPM2B from a uint32_t, typically a permanent handle */

TPM_RC TPM2B_CreateUint32(TPM2B *target, uint32_t source, uint16_t targetSize)
{
    TPM_RC rc = 0;
    
    if (rc == 0) {
	if (sizeof(uint32_t) > targetSize) {
	    if (tseVerbose) printf("TPM2B_CreateUint32: size %u greater than target %u\n",
				   (unsigned int)sizeof(uint32_t), targetSize);	
	    rc = TSE_RC_INSUFFICIENT_BUFFER;
	}
    }
    if (rc == 0) {
	uint32_t sourceNbo = htonl(source);
	memmove(target->buffer, (uint8_t *)&sourceNbo, sizeof(uint32_t));
	target->size = sizeof(uint32_t);
    }
    return rc;
}

/* TPM2B_StringCopy() copies a NUL terminated string (omiting the NUL) from source to target.

   It checks that the string will fit in targetSize.
*/

TPM_RC TPM2B_StringCopy(TPM2B *target, const char *source, uint16_t targetSize)
{
    TPM_RC rc = 0;
    size_t length;

    if (source != NULL) {
	if (rc == 0) {
	    length = strlen(source);
	    if (length > targetSize) {
		if (tseVerbose) printf("TPM2B_StringCopy: size %u greater than target %u\n",
				       (unsigned int)length, targetSize);	
		rc = TSE_RC_INSUFFICIENT_BUFFER;
	    }
	}
	if (rc == 0) {
	    target->size = length;
	    memcpy(target->buffer, source, length);
	}
    }
    else {
	target->size = 0;
    }
    return rc;
}

BOOL TPM2B_Compare(TPM2B *expect, TPM2B *actual)
{
    int 	irc;
    BOOL 	match = YES;

    if (match == YES) {
	if (expect->size != actual->size) {
	    match = NO;
	}
    }
    if (match == YES) {
	irc = memcmp(expect->buffer, actual->buffer, expect->size);
	if (irc != 0) {
	    match = NO;
	}
    }
    return match;
}

