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


t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *image = fopen(filename, "rb");

    //error when the file is not existing
    if (image == NULL) {
        printf("Error while opening the file, the file does not exist!\n");
        return NULL;
    }

    //extract image properties from the BMP header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, image);

    // Extract image properties from the header
    unsigned int width = *(unsigned int*)&header[18];
    unsigned int height = *(unsigned int*)&header[22];
    unsigned short colorDepth = *(unsigned short*)&header[28];
    unsigned int dataSize = *(unsigned int*)&header[34];

    //error when the file is not 8 bits deep
    if (colorDepth != 8) {
        printf("Error while opening the file, the file is not 8 bits deep!\n");
        fclose(image);
        return NULL;
    }

    //allocate memory for the BMP image 
    t_bmp8 *bmpImage = (t_bmp8 *)malloc(sizeof(t_bmp8));

    //initialize the image proprietes
    bmpImage->width = width;
    bmpImage->height = height;
    bmpImage->colorDepth = colorDepth;
    bmpImage->dataSize = dataSize;

    //allocate memory
    bmpImage->data = (unsigned char *)malloc(dataSize);

    //move to the pixel data location in the file and read the data
    fseek(image, *(unsigned int*)&header[10], SEEK_SET);
    fread(bmpImage->data, sizeof(unsigned char), dataSize, image);

    //close + print
    fclose(image);
    printf("Image loaded successfully!\n\n");
    return bmpImage;
}

void bmp8_saveImage(const char * filename, t_bmp8 * img) {
    //open the file for writing in binary mode
    FILE *file = fopen(filename, "wb");
    if (!file) {

        //if file cannot be opened, print an error message and return
        printf("Error during the opening of the file, (gdb work)");
        return;
    }

    //BMP header
    size_t header = fwrite(img->header, sizeof(unsigned char), 54, file);
    if (header != 54) {
        printf("Error occured while writing the header");
        fclose(file);
        return;
    }

    //BMP color table
    size_t ColorTable = fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    if (ColorTable != 1024) {

        printf("Error during the writing of color table");
        fclose(file);
        return;
    }

    //bMP pixel data
    size_t dataWritten = fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    if (dataWritten != img->dataSize) {

        printf("Error during the writing of pixel data");
        fclose(file);
        return;
    }

    //close the file
    fclose(file);
}


//to free the memory of the momery allocated
void bmp8_free(t_bmp8 * img){
    free(img->data);
    free(img);
}

//print the info of the image
void bmp8_printInfo(t_bmp8 * img){
    printf("Image Info\n    Width: %u\n    Height: %u\n    Color depth: %u\n    Datasize: %d\n      ", img->width, img->height, img->colorDepth, img->dataSize);
}


//negative function
void bmp8_negative(t_bmp8 * img){
    for (int i = 0; i < img->height; i++){
        for (int j = 0; j < img->width; j++){
            img->data[i+j*img->width] = 255- img->data[i+j*img->width];
        }
    }
}

void bmp8_brightness(t_bmp8 * img, int value){

}
void bmp8_threshold(t_bmp8 * img, int threshold);

//little main to chekc if its work
int main() {
    int i;
    char filename[256];
    t_bmp8 *image = NULL;

    //Start of the menu
    printf("Please choose an option:\n    1. Open an image\n    2. Save an image\n    3. Apply a filter\n    4. Display image information\n    5. Quit\n");
    printf(">>>>>Your choice: ");
    scanf("%d", &i);

    //First choice for the loading of the image
    if (i == 1) {
        printf("File path: ");
        scanf("%255s", filename);

        image = bmp8_loadImage(filename);
        bmp8_free(image);

    } else if (i == 2){
        if (!image){
            printf("No image, can't save the image");
        } else {
            bmp8_saveImage(filename, image);
        }
    } else if (i == 3){
        if (!image){
            printf("No image, can't apply a filter\n");
        } else {
        printf("Please choose an filter:\n    1. Negative\n    2. Brightness\n    3. Black and white\n    4. Box blur\n    5. Gaussian blur\n    6.Outline\n     7.Emboss\n     8.Return to previous menu\n");
        }
    } else  if (i == 4){
        if (!image){
            printf("No image, can't display info of the image");
        } else {
            bmp8_printInfo(image);
        }
    } else if (i == 5){
        return 0;
    }

    return 0;
}


