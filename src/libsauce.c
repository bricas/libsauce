#include "libsauce.h"

sauce* sauce_read_filename( char *filename ) {
    FILE *file = fopen( filename, "r" );
    if( file == NULL ) {
        return NULL;
    }

    sauce *record = sauce_read_file( file );
    fclose( file );
    return record;
}

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

    fread( record->id, sizeof( record->id ) - 1, 1, file );
    record->id[ sizeof( record->id ) - 1 ] = '\0';

    if( strcmp( record->id, SAUCE_ID ) != 0 ) {
        free( record );
        return;
    }

    fread( record->version, sizeof( record->version ) - 1, 1, file );
    record->version[ sizeof( record->version ) - 1 ] = '\0';
    fread( record->title, sizeof( record->title ) - 1, 1, file );
    record->title[ sizeof( record->title ) - 1 ] = '\0';
    fread( record->author, sizeof( record->author ) - 1, 1, file );
    record->author[ sizeof( record->author ) - 1 ] = '\0';
    fread( record->group, sizeof( record->group ) - 1, 1, file );
    record->group[ sizeof( record->group ) - 1 ] = '\0';
    fread( record->date, sizeof( record->date ) - 1, 1, file );
    record->date[ sizeof( record->date ) - 1 ] = '\0';
    fread( &(record->filesize), sizeof( record->filesize ), 1, file );
    fread( &(record->datatype), sizeof( record->datatype ), 1, file );
    fread( &(record->filetype), sizeof( record->filetype ), 1, file );
    fread( &(record->tinfo1), sizeof( record->tinfo1 ), 1, file );
    fread( &(record->tinfo2), sizeof( record->tinfo2 ), 1, file );
    fread( &(record->tinfo3), sizeof( record->tinfo3 ), 1, file );
    fread( &(record->tinfo4), sizeof( record->tinfo4 ), 1, file );
    fread( &(record->comments), sizeof( record->comments ), 1, file );
    fread( &(record->flags), sizeof( record->flags), 1, file );
    fread( record->filler, sizeof( record->filler ) - 1, 1, file );
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
        fread( id, sizeof( id ) - 1, 1, file );
        id[ sizeof( id ) - 1 ] = '\0';

        if( strcmp( id, COMMENT_ID ) != 0 ) {
            free( comment_lines );
            return;
        }

        for( i = 0; i < comments; i++ ) {
            char buf[ COMMENT_SIZE + 1 ] = "";

            fread( buf, COMMENT_SIZE, 1, file );
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

int sauce_write_filename( char *filename, sauce *record ) {
    FILE *file = fopen( filename, "r+" );

    if( file == NULL ) {
        return 1;
    }

    int rc = sauce_write_file( file, record );
    fclose( file );
    return rc;
}

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
    if( fseek( file, 0, SEEK_END ) == 0 ) {
        /* write record */
        return 0;
    }
    else {
        return 1;
    }
}

int sauce_remove_filename( char *filename ) {
    FILE *file = fopen( filename, "r+" );

    if( file == NULL ) {
        return 1;
    }

    int rc = sauce_remove_file( file );

    fclose( file );
    return rc;
}

int sauce_remove_file( FILE *file ) {
    sauce *record = sauce_read_file( file );

    if( record == NULL ) {
        return 0;
    }

    /* truncate to record->filesize */
    return 0;
}
