#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX 5

struct XmlType
{
    int START_DOCUMENT;
    int END_DOCUMENT ;
    int START_TAG;
    int END_TAG ;
    int TEXT ;
    int CDSECT ;
    int ENTITY_REF ;
    int IGNORABLE_WHITESPACE;
    int PROCESSING_INSTRUCTION;
    int COMMENT;
    int DOCDECL ;
    int ATTRIBUTE ;
};
struct  DataType
{
    int TYPE_NULL;
    int TYPE_STRING ;
    int TYPE_STRING_INTERNED ;
    int TYPE_BYTES_HEX ;
    int TYPE_BYTES_BASE64 ;
    int TYPE_INT ;
    int TYPE_INT_HEX ;
    int TYPE_LONG ;
    int TYPE_LONG_HEX ;
    int TYPE_FLOAT;
    int TYPE_DOUBLE;
    int TYPE_BOOLEAN_TRUE;
    int TYPE_BOOLEAN_FALSE;
};
char hex_to_char(char hexvalue)
{
    if(hexvalue>='0'&& hexvalue<='9')
    {
        return hexvalue - '0';
    }
    else if(hexvalue>='a'&& hexvalue<='f')
    {
        return hexvalue-'a' + 10;
    }   
    else if(hexvalue>='A'&& hexvalue<='F')
    {
        return hexvalue-'A' + 10;
    }
}
int push(char stack[MAX][80], int *top, char data[80])
{
    if(*top==MAX-1)
        return (-1);
    else
    {
        *top=*top+1;
        strcpy(stack[*top],data);
    }
}
int pop(char stack[MAX][80], int *top, char data[80])
{
    if(*top==-1)
        return (-1);
    else
    {
        strcpy(data,stack[*top]);
        *top=*top-1;
    }
}
int main()
{
    char stack[MAX][80];
    char str[80];
    int top=-1;
    struct XmlType s1={0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 15};
    struct DataType s2;
    s2.TYPE_NULL=1<<4;
    s2.TYPE_STRING=2<<4;
    s2.TYPE_STRING_INTERNED=3<<4;
    s2.TYPE_BYTES_HEX=4<<4;
    s2.TYPE_BYTES_BASE64=5<<4;
    s2.TYPE_INT=6<<4;
    s2.TYPE_INT_HEX=7<<4;
    s2.TYPE_LONG=8<<4;
    s2.TYPE_LONG_HEX=9<<4;
    s2.TYPE_FLOAT=10<<4;
    s2.TYPE_DOUBLE=11<<4;
    s2.TYPE_BOOLEAN_TRUE=12<<4;
    s2.TYPE_BOOLEAN_FALSE=13<<4;

    FILE *input_file, *output_file;
    char *buffer;
    long file_size;
    input_file=fopen("xmlfile.xml.abx", "rb");
    if(input_file==NULL)
    {
        printf("unable to open input abx file\n");
        return 1;
    }
    fseek(input_file, 0, SEEK_END);
    file_size=ftell(input_file);
    fseek(input_file, 0, SEEK_SET);

    buffer=(char*)malloc(file_size);
    if(buffer==NULL)
    {
        printf("failed to allocate buffer\n");
        fclose(input_file);
        return 1;
    }
    fread(buffer, 1, file_size, input_file);
    fclose(input_file);
    int bufferindex=0;
    printf("\n"); 
    while(bufferindex<file_size)
    {
        int token=buffer[bufferindex]&0xFF;
        int type=token & 0x0F;
        int data= token & 0xF0;
        if(type==s1.START_DOCUMENT && data==s2.TYPE_NULL) // printf("Error : START_DOCUMENT with an invalid data type");
        {
            //printf("start of document\n");
        }
        else if(type==s1.END_DOCUMENT && data==s2.TYPE_NULL)
        {
            //printf("END of document\n"); 
        }
        else if(type==s1.START_TAG && data==s2.TYPE_STRING_INTERNED)
        {
            //printf("START_TAG\n"); 
            bufferindex++;
            printf("<"); 
            if((buffer[bufferindex]&0xFF)==0xFF && (buffer[++bufferindex]&0xFF)==0xFF)
            {
                
                int len=buffer[bufferindex+=2]&0xFF;
                for(int i=0; i<len; i++)
                {
                    bufferindex++;
                    char ch=buffer[bufferindex];   
                    strncat(str, &ch, 1);
                    printf("%c",ch);       
                }  
            push(stack, &top, str);
            str[0]='\0';
            }  
        printf(">");
        }
        else if(type==s1.END_TAG && data==s2.TYPE_STRING_INTERNED)
        {
            pop(stack, &top, str);
            printf("</%s>",str);
            str[0]='\0';
        }
        else if(type==s1.TEXT && data==s2.TYPE_STRING)
        {
            int length=buffer[bufferindex+=2]&0xFF;
            for(int j=0; j<length; j++)
            {
                bufferindex++;
                char strtext=buffer[bufferindex];   
                printf("%c",strtext);
            }  
            
        }
        bufferindex++; 
    }
    printf("\n\n");
    free(buffer);
    return 0;
}