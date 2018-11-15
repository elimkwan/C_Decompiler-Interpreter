/*
****This program shows the version, instruction mnemonic corresponding to different opcode and machine codes of a text file.It can also shows the hexadecimal representation of a text file and decompile a machine code file to assembly code.
*/

#include <stdio.h>
#include <string.h>
#include "ConvertOpcodeHexfile.h"
#include "decompiler.h"

int main (int argc, char * argv[])
{
    //if only two arguement is presented, check if it is asking for version or hexdump
    if (argc==2)
    {
        if (!strcmp(argv[1],"--version"))
        {
            fprintf(stdout, "4.0\n");
            return 0;
        }
        else if(!strcmp(argv[1],"--hexdump"))
        {

            unsigned char content[9];
            OpenHexFile(content,stdin);
            return 0;
        }
        else
        {
            char* srcfilename = argv[1];
            FILE* srcfile;
            srcfile = fopen(srcfilename, "rb");
            if (srcfile == NULL)
            {
                fprintf(stderr,"Unable to open file \"%s\"\n", srcfilename);
                return 1;
            }
            
            //call the function to decomplie .out file into assembly code then print them out
            return (decompiler(srcfile));
            
        }
    }
    
    else if (argc == 3)
    {
        //call function to check the validity of the input opcode and print them out
        if (!strcmp(argv[1],"--opcode"))
        {
            return(PrintOpcode(argv[2]));
        }
        
        else if (!strcmp(argv[1],"--hexdump"))
        {
            char* srcfilename = argv[2];
            unsigned char content[9];
            
            FILE* srcfile;
            srcfile = fopen(srcfilename, "rb"); //read
            if (srcfile == NULL)
            {
                fprintf(stderr,"No such file \"%s\"\n", srcfilename);
                return 1;
            }
            
            //call the function to convert .txt file into hexideciaml number then print them out
            OpenHexFile(content,srcfile);
            
            fclose(srcfile);
            
            return 0;
        }

    }
    else
    {
        fprintf(stderr,"Usage:\n./asmdump.exe --version\n./asmdump.exe --opcode OPCODE\n./asmdump.exe --hexdump [FILE]\n./asmdump.exe FILE\n");
        return 1;
    }
    
}
