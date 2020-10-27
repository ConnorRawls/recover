#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    //Open memory file
    FILE *file = fopen(argv[1], "r");

    //Initialize JPEG counter
    int counter = 0;

    //Declare buffer
    char *buff;

    //Repeat process until end of card
    while(file != NULL)
    {
        //Read 512 bytes into buffer
        fread(buff, 512, 1, file);

        //If start of new JPEG
        if (buff[0] == 0xff && buff[1] == 0xd8 && buff[2] == 0xff && (buff[3] & 0xf0) == 0xe0)
        {
            //If first JPEG
            if (counter == 0)
            {
                sprintf(buff, "%03i.jpg", counter);
                FILE *img = fopen(buff, "w");

                //Start writing file
                fwrite(buff, 512, 1, img);

                counter++;
            }
            //Else
            else
            {
                //Close file and open up new one to continue writing
                fclose(file);

                sprintf(buff, "%03i.jpg", counter);
                FILE *img = fopen(buff, "w");

                fwrite(buff, 512, 1, img);

                counter++;
            }
        }
        //If already found JPEG
        else
        {
            FILE *img = fopen(buff, "w");

            //Keep writing
            fwrite(buff, 512, 1, img);
        }
    //Close remaining files
    fclose(file);
    }
}
