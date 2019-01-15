#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int searchfile(FILE *infile, char needle[20]);

int main(int argc, char *argv[])
{
    //Validate the input from command line.
    if(argc !=4){
        printf("Invalid input! Please check the numbers of input or space before running!");
        return 0;
    }

    FILE *infile,*outfile;
    char needle[20];
    char inputfile[30];
    unsigned long sizeofFile=0;
    int numofMatch=0;
    int rc;

    //Check the input file and output file whether they are valid.
    if ((infile = fopen(argv[1],"rb")) == NULL){
        printf("Cannot open input file.\n");
        exit(1);
    }
    if((outfile=fopen(argv[3],"wb")) == NULL){
        printf("Error opening the output file.\n");
        exit(1);
    }

    //Search the number of match
    strcpy(inputfile,argv[1]);
    strcpy(needle,argv[2]);
    numofMatch = searchfile(infile,needle);




    //Get file size
    fseek(infile,0,SEEK_END);
    sizeofFile = (unsigned long)ftell(infile);
    
    
    printf("Size of file is %lu\n",sizeofFile);
    printf("Number of matches = %d\n",numofMatch);

    //Print answer to the screen store it into the outfile
    fprintf(outfile,"Size of file is %lu\n",sizeofFile);
    fprintf(outfile,"Number of matches = %d\n",numofMatch);
    
    
    
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

//Search the substring needle within the input file. Using variable startIndex to get the rescan position when read chunks of file.
//Variable rescanIndex is used to reset the position if repeated character appear.
int searchfile(FILE *infile, char needle[20]){
    unsigned char buffer[100];
    char firstChar = needle[0];
    size_t bytes = 0;
    int match = 0;
    int flag = 0;
    int startIndex = 0;
    int rescanIndex = 0;
    int j =0;

    bytes = fread(buffer,1, sizeof(buffer), infile);
    while(bytes > 0){
        for(int i=0;i<bytes;){
            for(int j=0;needle[j]!='\0';j++){
				if(firstChar==buffer[i]){
                    startIndex = 100-i;
                }

                //If charcter matchs check whether this character is not in the first position and character is the same as first character in needle, then mark the rescan position for start scanning position in next iteration.
                if(needle[j]==buffer[i]){
                    if(buffer[i]==firstChar&&j!=0){
                        rescanIndex = i;
                    }
                    flag = 1;
                    if(needle[j+1]!='\0'){
                        i++;
                    }
                }
                else{
                    flag = 0;
                    break;
                }
            }
            //If find matches substring in text, increment the index.
            if(flag == 1){
                match++;
                startIndex = 0;
            }

            if(rescanIndex != 0){
                i = rescanIndex;
                rescanIndex = 0;
            }
            else{
                i++;
            }

        }
        //If the startIndex got value, then read from startIndex in next fread iteration.
        //Also need to check the 
        if(startIndex!=0 && startIndex!=100){
            fseek(infile, -startIndex,SEEK_CUR);
            bytes = fread(buffer,1, sizeof(buffer), infile);

            startIndex = 0;
        }else{
            bytes = fread(buffer,1, sizeof(buffer), infile);
        }

    }

    return match;
}
