typedef struct {
unsigned char header[54];
unsigned char colorTable[1024];
unsigned char * data;
unsigned int width;
unsigned int height;
unsigned int colorDepth;
unsigned int dataSize;
} t_bmp8;

t_bmp8 *bmp8_loadImage(const char *filename);
//To savec an image
void bmp8_saveImage(const char * filename, t_bmp8 * img);
//to free the memory of the momery allocated
void bmp8_free(t_bmp8 * img);
//print the info of the image
void bmp8_printInfo(t_bmp8 * img);
//negative function
void bmp8_negative(t_bmp8 * img);
//brghtness function
void bmp8_brightness(t_bmp8 * img, int value);
//threshold function
void bmp8_threshold(t_bmp8 *img, int threshold);
