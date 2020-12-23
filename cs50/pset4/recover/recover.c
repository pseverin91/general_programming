#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


typedef uint8_t BYTE;



int main(int argc, char *argv[])
{
    // Test input
    if (argc != 2)
    {
        printf("Inaccurate Input.");
        return 1;
    }
    
    // Open file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }
    
    // Check if first three bytes correspond to jpeg
    int count = 0;
    char filename[8];
    FILE *img = NULL;
    uint8_t buffer[512];
    
    // Loop through data blocks
    while (fread(buffer, sizeof(uint8_t), 512, file) || feof(file) == 0)
    {
        // Look for jpeg header
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // First jpeg
            if (count == 0)
            {
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(buffer), 1, img);
                
                count++;
            }
            
            // All other jpegs
            else if (count < 50)
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", count);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(buffer), 1, img);
                
                count++;
            }
        }
        
        // Write to jpeg file
        else if (count > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, img);
        }
    }
    fclose(img);
    fclose(file);
    return 0;
}
