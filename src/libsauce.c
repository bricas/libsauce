/**
 * @file
 * @author  Brian Cassidy <brian.cassidy@gmail.com>
 * @version 0.01
 *
 * @section LICENSE
 *
 * This software is copyright (c) 2010 by Brian Cassidy.
 * 
 * This is free software; you can redistribute it and/or modify it under
 * the same terms as the Perl 5 programming language system itself.
 * 
 * Terms of the Perl programming language system itself
 * 
 * a) the GNU General Public License as published by the Free
 *    Software Foundation; either version 1, or (at your option) any
 *    later version, or
 * b) the "Artistic License"
 *
 * @section DESCRIPTION
 *
 * A C library to read, write and remove SAUCE records.
 */

#include <stdio.h>
#include "libsauce.h"

/**
 * Read SAUCE via a filename
 * @param filename The filename to read
 * @return A SAUCE record
 */
sauce* sauce_read_filename( char *filename ) {
    FILE *file = fopen( filename, "r" );
    if( file == NULL ) {
        return NULL;
    }

    sauce *record = sauce_read_file( file );
    fclose( file );
    return record;
}

/**
 * Read SAUCE via a FILE pointer
 * @param file The FILE pointer to read
 * @return A SAUCE record
 */
sauce* sauce_read_file( FILE *file ) {
    sauce *record;

    record = malloc( sizeof *record );
    if( record != NULL ) {
        _read_record( file, record );
    }

    return record;
}

void _read_record( FILE *file, sauce *record ) {
    if( fseek( file, 0 - RECORD_SIZE, SEEK_END ) != 0 ) {
        free( record );
        return;
    }

    int read_status = fread( record->id, sizeof( record->id ) - 1, 1, file );
    record->id[ sizeof( record->id ) - 1 ] = '\0';

    if( read_status != 1 || strcmp( record->id, SAUCE_ID ) != 0 ) {
        free( record );
        return;
    }

    if( fread( record->version, sizeof( record->version ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE version" );
        exit( 1 );
    }
    record->version[ sizeof( record->version ) - 1 ] = '\0';
    if( fread( record->title, sizeof( record->title ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE title" );
        exit( 1 );
    }
    record->title[ sizeof( record->title ) - 1 ] = '\0';
    if( fread( record->author, sizeof( record->author ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE author" );
        exit( 1 );
    }
    record->author[ sizeof( record->author ) - 1 ] = '\0';
    if( fread( record->group, sizeof( record->group ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE group" );
        exit( 1 );
    }
    record->group[ sizeof( record->group ) - 1 ] = '\0';
    if( fread( record->date, sizeof( record->date ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE date" );
        exit( 1 );
    }
    record->date[ sizeof( record->date ) - 1 ] = '\0';
    if( fread( &(record->filesize), sizeof( record->filesize ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE filesize" );
        exit( 1 );
    }
    if( fread( &(record->datatype), sizeof( record->datatype ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE datatype" );
        exit( 1 );
    }
    if( fread( &(record->filetype), sizeof( record->filetype ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE filetype" );
        exit( 1 );
    }
    if( fread( &(record->tinfo1), sizeof( record->tinfo1 ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE tinfo1" );
        exit( 1 );
    }
    if( fread( &(record->tinfo2), sizeof( record->tinfo2 ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE tinfo2" );
        exit( 1 );
    }
    if( fread( &(record->tinfo3), sizeof( record->tinfo3 ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE tinfo3" );
        exit( 1 );
    }
    if( fread( &(record->tinfo4), sizeof( record->tinfo4 ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE tinfo4" );
        exit( 1 );
    }
    if( fread( &(record->comments), sizeof( record->comments ), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE comments" );
        exit( 1 );
    }
    if( fread( &(record->flags), sizeof( record->flags), 1, file ) != 1 ) {
        perror( "Unable to read SAUCE flags" );
        exit( 1 );
    }
    if( fread( record->filler, sizeof( record->filler ) - 1, 1, file ) != 1 ) {
        perror( "Unable to read SAUCE filler" );
        exit( 1 );
    }
    record->filler[ sizeof( record->filler ) - 1 ] = '\0';

    if( ferror( file ) != 0 ) {
        free( record );
        return;
    }

    if( record->comments > 0 ) {
        record->comment_lines = malloc( record->comments * sizeof( *record->comment_lines ) );

        if( record->comment_lines != NULL ) {
            _read_comments( file, record->comment_lines, record->comments );
        }
        else {
            free( record );
            return;
        }
    }
}

void _read_comments( FILE *file, char **comment_lines, int comments ) {
    int i;

    if( fseek( file, 0 - ( RECORD_SIZE + 5 + COMMENT_SIZE * comments ), SEEK_END ) == 0 ) {
        char id[ 6 ];
        if( fread( id, sizeof( id ) - 1, 1, file ) != 1 ) {
            perror( "COMNT record truncated (ID failed)" );
            exit( 1 );
        }
        id[ sizeof( id ) - 1 ] = '\0';

        if( strcmp( id, COMMENT_ID ) != 0 ) {
            free( comment_lines );
            return;
        }

        for( i = 0; i < comments; i++ ) {
            char buf[ COMMENT_SIZE + 1 ] = "";

            if( fread( buf, COMMENT_SIZE, 1, file ) != 1 ) {
                perror( "COMNT record truncated (comment line failed)" );
                exit( 1 );
            }
            buf[ COMMENT_SIZE ] = '\0';

            if( ferror( file ) == 0 ) {
                comment_lines[ i ] = strdup( buf );
                if( comment_lines[ i ] == NULL ) {
                    free( comment_lines );
                    return;
                }
            }
            else {
                free( comment_lines );
                return;
            }
        }

        return;
    }

    free( comment_lines );
    return;
}

/**
 * Write SAUCE via a filename
 * @param filename The filename in which to write the SAUCE record
 * @return 0 for success
 */
int sauce_write_filename( char *filename, sauce *record ) {
    FILE *file = fopen( filename, "r+" );

    if( file == NULL ) {
        return 1;
    }

    int rc = sauce_write_file( file, record );
    fclose( file );
    return rc;
}

/**
 * Write SAUCE via a FILE pointer
 * @param file The FILE pointer in which to write the SAUCE record
 * @return 0 for success
 */
int sauce_write_file( FILE *file, sauce *record ) {
    if( sauce_remove_file( file ) == 0 ) {
        int rc = _write_record( file, record );
        return rc;
    }
    else {
        return 1;
    }
}

int _write_record( FILE *file, sauce * record ) {
    if( fseek( file, 0, SEEK_END ) != 0 ) {
        return 1;
    }

    fwrite( "\032", 1, 1, file  );

    if( record->comments != 0 ) {
        fwrite( COMMENT_ID, 5, 1, file );
        int i;
        for( i = 0; i < record->comments; i++ ) {
            fwrite( record->comment_lines[ i ], COMMENT_SIZE, 1, file );
        }
    }

    fwrite( record->id, sizeof( record->id ) - 1, 1, file  );
    fwrite( record->version, sizeof( record->version ) - 1, 1, file );
    fwrite( record->title, sizeof( record->title ) - 1, 1, file );
    fwrite( record->author, sizeof( record->author ) - 1, 1, file );
    fwrite( record->group, sizeof( record->group ) - 1, 1, file );
    fwrite( record->date, sizeof( record->date ) - 1, 1, file );
    fwrite( &(record->filesize), sizeof( record->filesize ), 1, file );
    fwrite( &(record->datatype), sizeof( record->datatype ), 1, file );
    fwrite( &(record->filetype), sizeof( record->filetype ), 1, file );
    fwrite( &(record->tinfo1), sizeof( record->tinfo1 ), 1, file );
    fwrite( &(record->tinfo2), sizeof( record->tinfo2 ), 1, file );
    fwrite( &(record->tinfo3), sizeof( record->tinfo3 ), 1, file );
    fwrite( &(record->tinfo4), sizeof( record->tinfo4 ), 1, file );
    fwrite( &(record->comments), sizeof( record->comments ), 1, file );
    fwrite( &(record->flags), sizeof( record->flags), 1, file );
    fwrite( record->filler, sizeof( record->filler ) - 1, 1, file );

    return 0;
}

/**
 * Remove SAUCE via a filename
 * @param filename The filename from which to remove the SAUCE record
 * @return 0 for success
 */
int sauce_remove_filename( char *filename ) {
    FILE *file = fopen( filename, "r+" );

    if( file == NULL ) {
        return 1;
    }

    int rc = sauce_remove_file( file );

    fclose( file );
    return rc;
}

/**
 * Remove SAUCE via a FILE pointer
 * @param file The FILE pointer from which to remove the SAUCE record
 * @return 0 for success
 */
int sauce_remove_file( FILE *file ) {
    sauce *record = sauce_read_file( file );

    if( record == NULL || strcmp( record->id, SAUCE_ID ) != 0 ) {
        return 0;
    }

    if( ftruncate( fileno( file ), record->filesize ) != 0 ) {
        perror( "Truncate failed" );
        exit( 1 );
    }

    return 0;
}
