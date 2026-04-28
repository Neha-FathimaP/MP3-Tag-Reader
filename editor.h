#include<stdio.h>
#include"types.h"
Status edit_mode(int argc, char *argv[]);
Status read_and_validate_edit(char *argv[], char *frame_id);
Status copy_header(FILE *src, FILE *dest);
Status copy_remaining_data(FILE *src, FILE *dest);
Status do_editing(FILE *src, FILE *dest, char *frame_id, char *new_value);