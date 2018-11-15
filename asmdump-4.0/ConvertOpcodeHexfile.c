#include <stdio.h>
#include <string.h>
#include "ConvertOpcodeHexfile.h"
#include "decompiler.h"


int PrintOpcode(char *Input)
{
    
    int opcode =0;
    int check = sscanf(Input, "0x%2x", &opcode);
    int match = 0;
    int is2digit =0;
    int isHex2 = 0;
    int isHex3 = 0;
    int check0x=0;
    int x=0;
    char OpcodeList[] = {0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x14, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x30};
    char *carray[] = {"mov", "add", "sub", "mul", "div", "and", "or", "xor", "cmp", "mod", "not", "jmp", "jle", "jl", "je", "jne","jge","jg","syscall"};
    
    
    
    if (Input[0] == '0' && Input[1] =='x')
    {
        check0x=1;
    }
    
    if (strlen(Input)==4)
    {
        is2digit=1;
    }
    if ((Input[2] >=48 && Input[2] <=57) || (Input[2] >=97 && Input[2]<=102) )
    {
        isHex2=1;
    }
    
    if ((Input[3] >=48 && Input[3] <=57) || (Input[3] >=97 && Input[3]<=102) )
    {
        isHex3=1;
    }
    //valid Hex number: 48-57, 97-102
    if(Input[2] == '0' && Input[3] == '0')
    {
        fprintf(stdout, "invalid\n");
        return 0;
    }
    if (check != 0 && isHex2==1 && isHex3==1 && is2digit==1)
    {
        for (int a=0; a<19; a++)
        {
            if(opcode == OpcodeList[a])
            {
                x=a;
                match = 1;
            }
        }
    }
    //input instruction is one of the valid opcodes
    if (match == 1)
    {
        fprintf(stdout, "%s\n", carray[x]);
        return 0;
    }
    
    //attempt correct format opcode that does not exist
    else if (check != 0 && isHex2==1 && isHex3==1 && is2digit==1 && match == 0)
    {
        fprintf(stderr, "No such instruction \"%s\"\n", Input);
        return 1;
    }
    
    //invalid opcode: hex number but capitalized letter
    else if (check != 0 && isHex2==0 && isHex3==0)
    {
        //attempt invalid format opcode
        fprintf(stderr, "Invalid opcode string\"%s\"\n",Input);
        return 1;
    }
    
    //invalid opcode: first two number is hex number but exceed 2 digit
    else if (check != 0 && is2digit==0)
    {
        //attempt invalid format opcode
        fprintf(stderr, "Invalid opcode string \"%s\"\n",Input);
        return 1;
    }
    
    //invalid opcode: not hex number but argc is in the format of 0x___
    else if (check == 0 && check0x !=0)
    {
        //attempt invalid format opcode
        fprintf(stderr, "Invalid opcode string \"%s\"\n",Input);
        return 1;
    }
    return 0;
}

//function:hex file
void OpenHexFile(unsigned char content[9],FILE* srcfile)
{
    int ByteRead;
    int i=0;
    int count=0;
    int display_count=0;
    int temp=1;
    
    while(temp)
    {
        ByteRead=fread(content,1,8,srcfile);
        
        //first column
        display_count=8*count;
        fprintf(stdout,"%08x",display_count);
        
        if (ByteRead == 0)
        {
            fprintf(stdout,"\n");
            temp=0;
        }
        else if (ByteRead != 8)
        {
            fprintf(stdout," ");
            int no_of_space=0;
            i=0;
            for (i=0; i<ByteRead; i++)
            {
                //second column print hex
                fprintf(stdout," %02x",content[i]);
                
                //third column
                if (!(content[i] >= 0x20 && content[i] < 0x7F))
                {
                    content[i]='.';
                }
            }
            
            content[i] = '\0';
            
            no_of_space=8-ByteRead;
            
            i=0;
            for(i=0; i<no_of_space; i++)
            {
                fprintf(stdout,"   ");
            }
            
            //last column
            fprintf(stdout,"  |%s|\n",content);
            
            //first column
            display_count=8*count+ByteRead;
            fprintf(stdout,"%08x\n",display_count);
            temp=0;
        }
        else
        {
            fprintf(stdout," ");
            i=0;
            for (i=0; i<ByteRead; i++)
            {
                fprintf(stdout," %02x",content[i]);
                
                if (!(content[i] >= 0x20 && content[i] < 0x7F))
                {
                    
                    content[i]='.';
                }
            }
            
            content[i] = '\0';
            
            fprintf(stdout,"  |%s|\n",content);
            count=count+1;
            
        }
    }
    
}
