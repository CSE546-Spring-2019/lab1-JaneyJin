#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //Validate the input from command line.
    if(argc !=4){
        printf("Invalid input! Please check the numbers of input or space before running!");
        return 0;
    }
    
    FILE *infile,*outfile;
    char *inputfile, *outputfile, *buffer;
    char needle[20];
    unsigned long startIndex;
    unsigned long sizeofFile=0;
    unsigned long numofMatch=0;
    size_t bytes;
    int rc,flag=1;
    
    
    inputfile=argv[1];
    strcpy(needle,argv[2]);
    outputfile = argv[3];
    
    //Check the input file and output file whether they are valid.
    if ((infile = fopen(argv[1],"rb")) == NULL){
        printf("Cannot open input file %s.\n",inputfile);
        exit(1);
    }
    if((outfile=fopen(argv[3],"w")) == NULL){
        printf("Error opening the output file %s.\n",outputfile);
        exit(1);
    }
    buffer=(char *) malloc(1);
    
    //Search the number of match
    bytes=fread(buffer,1,1,infile);
    
    while(bytes>0){
        
        if(buffer[0]==needle[0]){
            //set the next character as next read position
            startIndex = ftell(infile);
            
            for(int i=1;needle[i]!='\0';i++){
                bytes = fread(buffer,1,1,infile);
                //Check the character is equal or the input file still have character left
                if(needle[i]!=buffer[0]||bytes ==0){
                    flag=0;
                    break;
                }
            }
            if(flag==1){
                numofMatch++;
            }
            
            
            //Relocate the start position, just go to next character.
            fseek(infile,startIndex,SEEK_SET);
            
        }
        
        //Reset flag and see next round it find match substring
        flag = 1;
        bytes= fread(buffer,1,1,infile);
    }
    free(buffer);
    
    //Get file size
    fseek(infile,0,SEEK_END);
    sizeofFile = (unsigned long)ftell(infile);
    
    printf("Size of file is %lu\n",sizeofFile);
    printf("Number of matches = %lu\n",numofMatch);
    
    //Print answer to the screen store it into the outfile
    fprintf(outfile,"Size of file is %lu\n",sizeofFile);
    fprintf(outfile,"Number of matches = %lu\n",numofMatch);
    
    //Close the file
    rc = fclose(infile);
    
    if(rc<0){
        perror("Close infile");
    }
    rc = fclose(outfile);
    if(rc<0){
        perror("Close outfile");
    }
    
    return 0;
    
    
}

