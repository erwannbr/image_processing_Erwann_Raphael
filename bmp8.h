#include <stdio.h>
#include <stdlib.h>


typedef struct {
unsigned char header[54];
unsigned char colorTable[1024];
unsigned char * data;
unsigned int width;
unsigned int height;
unsigned int colorDepth;
unsigned int dataSize;
} t_bmp8;

void bmp8_saveImage(const char * filename, t_bmp8 * img);
void bmp8_free(t_bmp8 * img);
void bmp8_printInfo(t_bmp8 * img);

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *image = fopen(filename, "rb");

    // Error when the file is not existing
    if (image == NULL) {
        printf("Error while opening the file, the file does not exist!\n");
        return NULL;
    }

    // Extract image properties from the BMP header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);

    // Extract image properties from the header
    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned short colorDepth = *(unsigned short*)&header[28];
    unsigned int dataSize = *(unsigned int*)&header[34];

    // Error when the file is not 8 bits deep
    if (colorDepth != 8) {
        printf("Error while opening the file, the file is not 8 bits deep!\n");
        fclose(image);
        return NULL;
    }

    // Allocate memory for the BMP image structure
    t_bmp8 *bmpImage = (t_bmp8 *)malloc(sizeof(t_bmp8));

    // Initialize the image properties
    bmpImage->width = width;
    bmpImage->height = height;
    bmpImage->colorDepth = colorDepth;
    bmpImage->dataSize = dataSize;

    // Allocate memory for pixel data
    bmpImage->data = (unsigned char *)malloc(dataSize);

    // Move to the pixel data location in the file and read the data
    fseek(image, *(unsigned int*)&header[10], SEEK_SET);
    fread(bmpImage->data, sizeof(unsigned char), dataSize, image);

    fclose(image);
    printf("Image loaded successfully!\n");
    return bmpImage;
}



void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    // Open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (!file) {
        // If file cannot be opened, print an error message and return
        perror("Error opening file");
        return;
    }

    // Write the BMP header (54 bytes)
    size_t headerWritten = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (headerWritten != 54) {
        // Error in writing header
        perror("Error writing header");
        fclose(file);
        return;
    }

    // Write the color table (1024 bytes for 256 grayscale colors)
    size_t colorTableWritten = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (colorTableWritten != 1024) {
        // Error in writing color table
        perror("Error writing color table");
        fclose(file);
        return;
    }

    // Write the pixel data
    size_t dataWritten = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (dataWritten != img->dataSize) {
        // Error in writing data
        perror("Error writing pixel data");
        fclose(file);
        return;
    }

    // Close the file
    fclose(file);
}

t_bmp8 *bmp8_loadImage(lena_gray);