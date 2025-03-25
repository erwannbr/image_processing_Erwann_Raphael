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

//brghtness function
void bmp8_brightness(t_bmp8 * img, int value){
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

            if ((img->data[index] + value) > 255) {
                img->data[index] = 255;
            } else if ((img->data[index] + value) < 0) {
                img->data[index] = 0;
            } else {
                img->data[index] = img->data[index] + value;
            }
        }
    }
}

//threshold function
void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int index = i * img->width + j;

            if (img->data[index] >= threshold) {
                img->data[index] = 255;
            } else {
                img->data[index] = 0;
            }
        }
    }
}

//little main to chekc if its work
int main() {
    int choice, filterChoice, run = 1, bright, thresh;
    char filename[256], saveFilename[256];
    t_bmp8 *image = NULL;

    //Biggest loop to run the programme
    while (run) {
        printf("\nPlease choose an option:\n    1. Open an image\n    2. Save an image\n    3. Apply a filter\n    4. Display image information\n    5. Quit\n");
        printf("\n>>>>> Your choice: ");
        scanf("%d", &choice);

        //switch case because it is logical (more than while)
        switch (choice) {
            case 1:
                //To open a file
                printf("Enter file path: ");
                scanf("%255s", filename);
                image = bmp8_loadImage(filename);
                break;

            case 2:
                //To save an image
                if (!image) {
                    printf("No image loaded, can't save the image.\n");
                } else {
                    printf("Enter save file path: ");
                    scanf("%255s", saveFilename);
                    bmp8_saveImage(saveFilename, image);
                }
                break;

            case 3:
                //To save a filter
                if (!image) {
                    printf("No image loaded, can't apply a filter.\n");
                } else {
                    printf("Choose a filter:\n    1. Negative\n    2. Brightness\n    3. Black and white\n    4. Box blur\n    5. Gaussian blur\n    6. Outline\n    7. Emboss\n    8. Return to previous menu\n");
                    printf(">>>>> Your choice: ");
                    scanf("%d", &filterChoice);

                    //For the negative filter
                    if (filterChoice == 1) {
                        bmp8_negative(image);
                        printf("Negative filter applied.\n");

                    //For the brightness filter
                    } else if (filterChoice == 2) {
                        printf("Whats the value of the bright you wanna change ? ");
                        scanf("%d", &bright);

                        bmp8_brightness(image,bright);
                        printf("Brightness filter applied.\n");

                    //For the treshold
                    } else if (filterChoice == 3) {
                        printf("Which color of pixel you wanna bright ? (0-255) :");
                        scanf("%d", &thresh);

                        bmp8_threshold(image, thresh);
                        printf("Brightness filter applied.\n");
                    }
                }
                break;

            case 4:
                //Display all the information
                if (!image) {
                    printf("No image loaded, can't display info.\n");
                } else {
                    bmp8_printInfo(image);
                }
                break;

            case 5:
                //To exit the programme
                run = 0;
                if (image) {
                    bmp8_free(image);
                }
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice. Please enter a number between 1 and 5.\n");
        }
    }
    return 0;
}

