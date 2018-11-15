#include <stdio.h>
#include <string.h>
#include "decompiler.h"

//define a type: for a function to return serveral integer
struct ReturnMultiples
{
    int x;
    int y;
    int z;
}typedef ReturnMultiples;

ReturnMultiples convertToBinary(int ip, unsigned char memory[101], ReturnMultiples result);
int BinaryInstruction(int ip, unsigned char memory[101], ReturnMultiples result);

int decompiler(FILE* srcfile)
{
    unsigned char memory[101];
    int ip;
    int ByteRead;
    int i=0;
    int j=0;
    char k;
    int START=0;
    int haveNewline=0;
    int error=0;
    int temp=0;
    ReturnMultiples result={0,0,0};
    int isInstruction=30;
    int wasData=0;
    unsigned int InstructionNum[] ={0x01, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x14, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x30};
    char* InstructionWord []={"mov", "add", "sub", "mul", "div", "and", "or", "xor", "cmp", "mod", "not", "jmp", "jle", "jl ", "je ", "jne","jge","jg","syscall"};
    
    i=0;
    while((k=fgetc(srcfile)) !=EOF)
    {
        memory[i]=k;
        i=i+1;
    }
    //i is the end of file point
    
    START= memory[i-1];//define START, the location of the first instuction
    ip=START;
    ByteRead=i;
    
    //if there are some strings before the first instuction, print them out
    if (START !=0x0)
    {
        fprintf(stdout,"         db \"");
        
        j=0;
        for(j=0;j<ip;j++)
        {
            if(memory[j]=='\n')
            {
                fprintf(stdout,"\\n");
                haveNewline=1;
            }
            else
            {
                fprintf(stdout,"%c",memory[j]);
            }
        }
        
        fprintf(stdout,"\"\n");
    }
    
    wasData=0;
    temp=1;
    while(temp)
    {
        //checking the type of one byte
        i=0;
        isInstruction = 30;
        for(i=0;i<19;i++)
        {
            if(memory[ip]==InstructionNum[i])
            {
                isInstruction=i;
            }
        }
        if (ip==ByteRead-2)//printing out space for non-first line instructions
        {
            fprintf(stdout,"\n         %s\n",InstructionWord[isInstruction]);
            temp=0;
        }
        else if( wasData==1 && isInstruction == 30)  //invalid opcode
        {
            fprintf(stderr,"Invalid opcode 0x%02x at pos 0x%02x\n",memory[ip],ip);
            error=1;
            temp=0;
        }
        else//printing binary instructions
        {
            wasData=0;
            if (ip ==START)
            {
                fprintf(stdout,"main:    %s",InstructionWord[isInstruction]);//printing out first line instructions
            }
            else
            {
                fprintf(stdout,"\n         %s",InstructionWord[isInstruction]);
            }

            if (isInstruction < 10) //binary instruction --> need to inspect binary form
            {
                ip=ip+1;
                result=convertToBinary(ip,memory,result);
                ip=BinaryInstruction(ip,memory,result);
            }
            else if (isInstruction<18)//NOT instruction, jump
            {
                ip=ip+1;
                fprintf(stdout," 0x%02x",memory[ip]);
            }
            //syscall no need to print any more thing
            ip=ip+1;
            wasData=1;
        }
    }
    
    fclose(srcfile);
    if (error==1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
ReturnMultiples convertToBinary(int ip, unsigned char memory[101],ReturnMultiples result)
{
    
    int dividends;
    int j=7;
    int bit[8];
    
    dividends=memory[ip]; //getting the binary form of the byte pointed to by the current memory address
    
    for (j=7;j>=0;j=j-1)
    {
        bit[j]= dividends%2;
        dividends = dividends/2;
    }
    
    //calculate the ascii code of the case number,register,immediate
    result.x= bit[1]*1 + bit[0]*2;
    result.y= bit[4]*1 + bit[3]*2 + bit[2]*4;
    result.z= bit[7]*1 + bit[6]*2 + bit[5]*4;
    
    return result;
    
}

int BinaryInstruction(int ip, unsigned char memory[101], ReturnMultiples result)
{
    int CaseNo1=0;
    int CaseNo2=0;
    
    CaseNo1=result.x;//case number, numbering start from zero
    CaseNo2=result.x + result.y;
    
    if (CaseNo1==3 && CaseNo2 == 3)//case4 register:immediant
    {
        ip=ip+1;
        
        fprintf(stdout," ");
        checkRegister(result.z);
        fprintf(stdout,",0x%02x",memory[ip]);//print out memory of the next next byte
    }
    else if(CaseNo1==3 && CaseNo2 == 4)//case5
    {
        ip=ip+1;
        
        fprintf(stdout," ");
        checkRegister(result.z);
        fprintf(stdout,",(0x%02x)",memory[ip]);
    }
    else if(CaseNo1==3 && CaseNo2 == 5)//case6
    {
        ip=ip+1;
        
        fprintf(stdout," ");
        fprintf(stdout,",(0x%02x)",memory[ip]);
        checkRegister(result.z);
    }
    else if(CaseNo1==0)//case 1
    {
        fprintf(stdout," ");
        checkRegister(result.y);
        fprintf(stdout,",");
        checkRegister(result.z);
    }
    else if(CaseNo1==1)//case 2
    {
        fprintf(stdout," ");
        checkRegister(result.y);
        fprintf(stdout,",(");
        checkRegister(result.z);
        fprintf(stdout,")");
    }
    else if(CaseNo1==2)//case 3
    {
        fprintf(stdout," (");
        checkRegister(result.y);
        fprintf(stdout,"),");
        checkRegister(result.z);
    }
    return ip;
}

void checkRegister(register_no)
{
    
    if(register_no==0)
    {
        //%a
        fprintf(stdout,"%%a");
    }
    else if(register_no==1)
    {
        //%b
        fprintf(stdout,"%%b");
    }
    else if(register_no==2)
    {
        //%c
        fprintf(stdout,"%%c");
    }
    else if(register_no==3)
    {
        //%d
        fprintf(stdout,"%%d");
    }
    else if(register_no==4)
    {
        //%ip
        fprintf(stdout,"%%ip");
    }
    else if(register_no==5)
    {
        //%sp
        fprintf(stdout,"%%sp");
    }
    else if(register_no==6)
    {
        //%bp
        fprintf(stdout,"%%bp");
    }
    else if(register_no==7)
    {
        //%flag
        fprintf(stdout,"%%flag");
    }
}
