
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "types.h"
#include"reader.h"
Status read_and_validate(char*argv[])
{

    if(argv[2]==NULL)
    {
        printf("file not passed\n");
        return failure;
    }
    if(strstr(argv[2],".mp3")==NULL)
    {
        printf("Invalid file name\n");
        return failure;
    }
    FILE* fptr=fopen(argv[2],"r");
    if(fptr==NULL)
    {
        printf("file doesnt exist\n");
        return failure;
    }
    fseek(fptr,0,SEEK_END);
    printf("file size:%lu\n",ftell(fptr));
    fclose(fptr);

    return success;
}
Status skip_header(FILE*fptr)
{
    //printf("before skipping header:%lu\n",ftell(fptr));
    //skip 10 byte header
    fseek(fptr,10,SEEK_SET);
    printf("skipped header\n");
    //printf("after skipping header:%lu\n",ftell(fptr));
    

    return success;
}

Status do_reading(FILE* fptr)
{
    char tag[5];
    unsigned char size[4];
    int tag_size;

    //read 4 byte tag
    if(fread(tag,1,4,fptr)!=4)
    return failure;
    tag[4]='\0';
    //printf("4byte tag read successfully\n");
    //printf("offset:%lu\n",ftell(fptr));
    if(tag[0] == 0)   
        return failure;
    //read 4 byte size
    if(fread(size,1,4,fptr)!=4)
    return failure;
    //printf("4byte size read successfully\n");
    //printf("offset:%lu\n",ftell(fptr));
    //convert to little endian
    tag_size=(size[0]<<24)|(size[1]<<16)|(size[2]<<8)|(size[3]);
    

    //skip flag
    fseek(fptr,2,SEEK_CUR);
    //printf("skipped 2 byte flag\n");
    //printf("offset:%lu\n",ftell(fptr));

    //printf("Calculated tag_size: %d\n", tag_size);
    //read tag data
    //char data[tag_size];
    char*data=malloc(tag_size);
    if(data==NULL)
    {
        printf("memory allocation failed\n");
        return failure;
    }

    if(fread(data,1,tag_size,fptr)!=tag_size)
    return failure;
    data[tag_size]='\0';
    //printf("tag data read successfully\n");
    //printf("offset:%lu\n",ftell(fptr));
    if (strcmp(tag,"TIT2") ==0||
        strcmp(tag,"TALB")==0 ||
        strcmp(tag,"TYER")==0 ||
        strcmp(tag,"TCON")==0 ||
        strcmp(tag,"COMM") ==0||
        strcmp(tag,"TPE1")==0)
        {
            printf("%s:",tag);
            //printf("Tag data:");
            if(data[0]==1)
            {
                for(int i=3;i<tag_size;i++)
                {
                    if(data[i]!=0)
                    printf("%c",data[i]);
                }

            }
            else
            {
                for(int i=1;i<tag_size;i++)
                {
                    if(data[i]!=0)
                    printf("%c",data[i]);
                }
            }
            printf("\n");
        }

    return success;
}
  

   