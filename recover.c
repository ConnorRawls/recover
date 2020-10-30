#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    //Check for correct command line usage
    if(argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    //Declares file pointer and open memory card
    FILE *file = fopen(argv[1], "r");

    //Check if file can be read
    if(file == NULL)
    {
        printf("Error: Could not read file.\n");
        return 1;
    }

    //Declares type BYTE of stdint.h and buffer
    typedef uint8_t BYTE;
    BYTE buffer[BLOCK_SIZE];

    //JPEG counter
    int counter = 0;

    //Name of files, ###.jpg
    char file_name[100];

    //The actual data
    FILE *image;

    //First JPEG?
    bool first_JPEG = false;

    //JPEG found boolean
    bool found = false;

    int bytes_read;

    //Repeat until end of card
    while(true)
    {
        //Read 512 bytes into buffer
        bytes_read = fread(buffer, sizeof(BYTE), BLOCK_SIZE, file);

        //If end of file
        if(bytes_read == 0)
        {
            //Close loop
            break;
        }

        //If start of new JPEG
        if(buffer[0]== 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //If first JPEG
            if(!first_JPEG)
            {
                first_JPEG = true;
            }

            else
            {
                //Close current file and open new file
                fclose(image);
            }

            sprintf(file_name, "%03i.jpg", counter+1);
            image = fopen(file_name, "w");
            found = true;
            counter++;
        }

        //Else
        else
        {
            //If already found JPEG
            if(found == true)
            {
                //Keep writing to it
                fwrite(buffer, sizeof(BYTE), bytes_read, image);
            }
        }
    }

    fclose(file);
    fclose(image);
    return 0;
}
