#include "../src/libsauce.h"

int main( int argc, char *argv[] ) {
    sauce *record = sauce_read_filename( argv[ 1 ] );

    if( record == NULL ) {
        printf( "Error!\n" );
        exit( EXIT_FAILURE );
    }

    if( strcmp( record->id, SAUCE_ID ) != 0 ) {
        printf( "File does not have a SAUCE record\n" );
        return 0;
    }

    printf( "%9s: [%s]\n", "id", record->id );
    printf( "%9s: [%s]\n", "version", record->version );
    printf( "%9s: [%s]\n", "title", record->title );
    printf( "%9s: [%s]\n", "autor", record->author );
    printf( "%9s: [%s]\n", "group", record->group );
    printf( "%9s: [%s]\n", "date", record->date );
    printf( "%9s: [%li]\n", "filesize", record->filesize );
    printf( "%9s: [%d]\n", "datatype", record->datatype );
    printf( "%9s: [%d]\n", "filetype", record->filetype );
    printf( "%9s: [%d]\n", "tinfo1", record->tinfo1 );
    printf( "%9s: [%d]\n", "tinfo2", record->tinfo2 );
    printf( "%9s: [%d]\n", "tinfo3", record->tinfo3 );
    printf( "%9s: [%d]\n", "tinfo4", record->tinfo4 );
    printf( "%9s: [%d]\n", "comments", record->comments );
    if( record->comments > 0 ) {
        int i;
        for( i = 0; i < record->comments; i++ ) {
            printf( "%9s: [%s]\n", "", record->comment_lines[ i ] );
        }
    }
    printf( "%9s: [%d]\n", "flags", record->flags );
    printf( "%9s: [%s]\n", "filler", record->filler );

    return 0;
}
