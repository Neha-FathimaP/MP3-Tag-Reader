#include<stdio.h>
#include<string.h>
#include"types.h"
#include"reader.h"
#include"editor.h"
int main(int argc,char *argv[])
{
    int ret=check_operation_type(argc,argv);
    if(ret==unsupported)
    {
        printf("ERROR: Invalid operation type\n");
        return 1;

    }
    
    if(ret==reader)
    {
    
        ret=read_and_validate(argv);
        if(ret==failure)
        {
            printf("Invalid arg\n");
            return 1;
        }
        FILE* fptr=fopen(argv[2],"r");
        if(fptr==NULL)
        {
            printf("Unable to open file\n");
            return 1;
        }
        ret=skip_header(fptr);
        if(ret==failure)
        {
            printf("failed to skip header\n");
            return 1;
        }
      
       while (do_reading(fptr) == success);
       printf("Reader successfull\n");
       fclose(fptr);

       
    }

    if(ret==editor)
    {
        ret = edit_mode(argc, argv);
    if(ret == failure)
    {
        printf("Edit failed\n");
        return 1;
    }

    }

}

OperationType check_operation_type(int argc,char *argv[])
{
    if(argc<2)
    return unsupported;
    if(argv[1]==NULL)
    {
        return unsupported;
    }
    if(strcmp(argv[1],"-r")==0)
    {
        return reader;
    }
    if(strcmp(argv[1],"-e")==0)
    {
        return editor;
    }

    return unsupported;
}