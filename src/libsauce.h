#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef LIBSAUCE_H
#define LIBSAUCE_H

#define RECORD_SIZE  128
#define COMMENT_SIZE 64
#define SAUCE_ID     "SAUCE"
#define COMMENT_ID   "COMNT"

typedef struct {
    char             id[ 6 ];
    char             version[ 3 ];
    char             title[ 36 ];
    char             author[ 21 ];
    char             group[ 21 ];
    char             date[ 9 ];
    long             filesize;
    unsigned char    datatype;
    unsigned char    filetype;
    unsigned short   tinfo1;
    unsigned short   tinfo2;
    unsigned short   tinfo3;
    unsigned short   tinfo4;
    unsigned char    comments;
    unsigned char    flags;
    char             filler[ 23 ];
    char           **comment_lines;
} sauce;

sauce* sauce_read_filename( char *filename );
sauce* sauce_read_file( FILE *file );
void _read_record( FILE *file, sauce *record );
void _read_comments( FILE *file, char **comment_lines, int comments );
int sauce_write_filename( char *filename, sauce *record );
int sauce_write_file( FILE *file, sauce *record );
int _write_record( FILE *file, sauce * record );
int sauce_remove_filename( char *filename );
int sauce_remove_file( FILE *file );

#endif
