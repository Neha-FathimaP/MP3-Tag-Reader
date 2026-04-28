#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "editor.h"

Status edit_mode(int argc, char *argv[])
{
    char frame_id[5];

    int ret=read_and_validate_edit(argv,frame_id);
    if(ret==failure)
    {
        return failure;
    }
        

    char *new_value = argv[3];
    

    FILE *src = fopen(argv[4], "r");
    if(src == NULL)
    {
        printf("Unable to open source file\n");
        return failure;
    }
    fseek(src,0,SEEK_END);
    printf("file size before editing:%lu\n",ftell(src));
    rewind(src);

    FILE *dest = fopen("new.mp3", "wb");
    if(dest == NULL)
    {
        printf("Unable to create new file\n");
        return failure;
    }


    ret=copy_header(src, dest);
    //printf("offset after copying header:%lu\n",ftell(dest));
    if(ret==failure)
    {
        printf("copying header failed\n");
        return failure;
    }
        

    while (do_editing(src, dest,frame_id, new_value) == success);
    
    ret=copy_remaining_data(src, dest);
    //printf("%lu\n",ftell(dest));
    if(ret==failure)
    {
        printf("copy remaining data failed\n");
        return failure;
    }
        

    fclose(src);
    fclose(dest);
    remove(argv[4]);
    rename("new.mp3", argv[4]);

    printf("Edit completed successfully\n");
    return success;

}

Status read_and_validate_edit(char *argv[], char *frame_id)
{
    if(argv[2]==NULL)
    {
        printf("ERROR:edit option not given\n");
        return failure;
    }
    if(argv[3] == NULL)
    {
        printf("ERROR: New value not provided\n");
        return failure;
    }

    if(argv[4] == NULL)
    {
        printf("ERROR: File name not provided\n");
        return failure;
    }
    if(strstr(argv[4], ".mp3") == NULL)
    {
        printf("ERROR: Invalid file format\n");
        return failure;
    }

    if(strcmp(argv[2], "-t") == 0)
        strcpy(frame_id, "TIT2");
    else if(strcmp(argv[2], "-A") == 0)
        strcpy(frame_id, "TALB");
    else if(strcmp(argv[2], "-y") == 0)
        strcpy(frame_id, "TYER");
    else if(strcmp(argv[2], "-c") == 0)
        strcpy(frame_id, "TCON");
    else if(strcmp(argv[2], "-m") == 0)
        strcpy(frame_id, "COMM");
    else if(strcmp(argv[2], "-a") == 0)
        strcpy(frame_id, "TPE1");
    else
    {
        printf("Invalid edit option\n");
        return failure;
    }

    return success;
}

Status copy_header(FILE *src, FILE *dest)
{
    char header[10];

    if(fread(header, 1, 10, src)!=10)
    return failure;
        

    fwrite(header, 1, 10, dest);

    return success;
}



Status do_editing(FILE*src,FILE*dest,char *frame_id, char *new_value)
{
    char tag[5];
    unsigned char size[4];
    int tag_size;

    //read 4 byte tag
    if(fread(tag,1,4,src)!=4)
    return failure;
    
    tag[4]='\0';
    if(tag[0] == 0)
    {
        fseek(src, -4, SEEK_CUR);
        return failure;
    }


    if(fread(size,1,4,src)!=4)
    return failure;
   
    tag_size=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|(size[3]);

    //read 2 byte flag
    char flag[2];
    if(fread(flag,1,2,src)!=2)
    return failure;


    char*data=malloc(tag_size);
    if(data==NULL)
    {
        printf("memory allocation failed\n");
        return failure;
    }
    
    if(fread(data,1,tag_size,src)!=tag_size)
    return failure;

    if(strcmp(tag,frame_id)==0)
   
        {
            fwrite(tag,1,4,dest);
            int new_size=strlen(new_value)+1;
            unsigned char new_size_buf[4];
            new_size_buf[0] = (new_size >> 24) & 0xFF;
            new_size_buf[1] = (new_size >> 16) & 0xFF;
            new_size_buf[2] = (new_size >> 8) & 0xFF;
            new_size_buf[3] = new_size & 0xFF;
            fwrite(new_size_buf, 1, 4, dest);
            fwrite(flag, 1, 2, dest);
            fputc(0,dest);
            fwrite(new_value,1,strlen(new_value),dest);
        }
    else
    {
        
        fwrite(tag, 1, 4, dest);
        fwrite(size, 1, 4, dest);
        fwrite(flag, 1, 2, dest);
        fwrite(data, 1, tag_size, dest);
    }
    free(data);
    return success;

}

Status copy_remaining_data(FILE *src, FILE *dest)
{
    int ch;
    while((ch = fgetc(src)) != EOF)
    {
        fputc(ch, dest);
    }
    printf("file size after editing:%lu\n",ftell(dest));
    return success;
}
