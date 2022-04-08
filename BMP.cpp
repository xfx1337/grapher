#include "BMP.h"
#include <cstring>

void BMP::close() {
	fclose(this->file);
}

int BMP::open() {
    this->file = fopen(this->filename.c_str(), "rb");

    if (this->file == NULL)
        return -1;

    fseek(this->file, 0, SEEK_END);
    int sz = ftell(this->file);
    fseek(this->file, 0, SEEK_SET);

    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, this->file);
    int data_offset = *(int*)(&info[0x0A]);

    if (data_offset > 54) {
        fseek(this->file, (long int)(data_offset - 54), SEEK_CUR);
    }

    // extract image height and width from header
    this->width = *(int*)&info[18];
    this->height = *(int*)&info[22];

    this->bytesPerPixel = (sz - 54) / (this->width * this->height);

    int row_padded = (this->width * this->bytesPerPixel + this->bytesPerPixel) & (~this->bytesPerPixel);
    unsigned char* data = new unsigned char[row_padded];
    unsigned char tmp;

    this->pixels.resize(this->height);
    for (int i = 0; i < this->height; i++) {
        this->pixels.at(i).resize(this->width);
    }

    for (int i = 0; i < this->height; i++)
    {
        fread(data, sizeof(unsigned char), row_padded, this->file);
        for (int j = 0; j < this->width * this->bytesPerPixel; j += this->bytesPerPixel)
        {
            tmp = data[j];
            data[j] = data[j + 2];
            data[j + 2] = tmp;

            this->pixels.at(i).at(j / this->bytesPerPixel).b = (int)data[j];
            this->pixels.at(i).at(j / this->bytesPerPixel).g = (int)data[j + 1];
            this->pixels.at(i).at(j / this->bytesPerPixel).r = (int)data[j + 2];
        }
    }
    fclose(this->file);
    return 0;
}


void BMP::write(string filename, vector<vector<RGB>> pixels) {
    unsigned char* pixelsar = (unsigned char*)malloc(this->width * this->height * this->bytesPerPixel);
    memset(pixelsar, 0, this->width * this->height * this->bytesPerPixel);
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            pixelsar[i * this->width * bytesPerPixel + j*bytesPerPixel] = (unsigned char)pixels[i][j].r;
            pixelsar[i * this->width * bytesPerPixel + j*bytesPerPixel + 1] = (unsigned char)pixels[i][j].g;
            pixelsar[i * this->width * bytesPerPixel + j*bytesPerPixel + 2] = (unsigned char)pixels[i][j].b;
        }

    }

    FILE* outputFile = fopen(filename.c_str(), "wb");
    const char* BM = "BM";
    fwrite(&BM[0], 1, 1, outputFile);
    fwrite(&BM[1], 1, 1, outputFile);
    int paddedRowSize = (int)(4 * ceil((float)this->width / 4.0f)) * this->bytesPerPixel;
    unsigned int fileSize = paddedRowSize * this->height + HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(&fileSize, 4, 1, outputFile);
    unsigned int reserved = 0x0000;
    fwrite(&reserved, 4, 1, outputFile);
    unsigned int dataOffset = HEADER_SIZE + INFO_HEADER_SIZE;
    fwrite(&dataOffset, 4, 1, outputFile);

    unsigned int infoHeaderSize = INFO_HEADER_SIZE;
    fwrite(&infoHeaderSize, 4, 1, outputFile);
    fwrite(&this->width, 4, 1, outputFile);
    fwrite(&this->height, 4, 1, outputFile);
    short planes = 1; //always 1
    fwrite(&planes, 2, 1, outputFile);
    short bitsPerPixel = this->bytesPerPixel * 8;
    fwrite(&bitsPerPixel, 2, 1, outputFile);

    unsigned int compression = NO_COMPRESION;
    fwrite(&compression, 4, 1, outputFile);
    unsigned int imageSize = this->width * this->height * this->bytesPerPixel;
    fwrite(&imageSize, 4, 1, outputFile);
    unsigned int resolutionX = 11811; //300 dpi
    unsigned int resolutionY = 11811; //300 dpi
    fwrite(&resolutionX, 4, 1, outputFile);
    fwrite(&resolutionY, 4, 1, outputFile);
    unsigned int colorsUsed = MAX_NUMBER_OF_COLORS;
    fwrite(&colorsUsed, 4, 1, outputFile);
    unsigned int importantColors = ALL_COLORS_REQUIRED;
    fwrite(&importantColors, 4, 1, outputFile);
    int i = 0;
    int unpaddedRowSize = this->width * this->bytesPerPixel;
    unsigned char padzero[1] = { 0 };

    for (i = this->height-1; i > -1; i--)
    {
        int pixelOffset = ((this->height - i) - 1) * unpaddedRowSize;
        fwrite(&pixelsar[pixelOffset], 1, unpaddedRowSize, outputFile);
        fwrite(padzero, 1, paddedRowSize-unpaddedRowSize, outputFile);
    }
    fclose(outputFile);
}


void BMP::genGrayScaleImage() {
    this->g_pixels.resize(this->height);
    for (int i = 0; i < this->height; i++) {
        this->g_pixels.at(i).resize(this->width);
    }

    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            int c = (int)(pixels[i][j].r + pixels[i][j].g + pixels[i][j].b) / 3;
            this->g_pixels[i][j] = {c,c,c};
        }
    }

}

void BMP::genDigitImage(int threshold, bool invert) {
    this->m_pixels.resize(this->height);
    for (int i = 0; i < this->height; i++) {
        this->m_pixels.at(i).resize(this->width);
    }
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if ((this->g_pixels[i][j].r + this->g_pixels[i][j].g + this->g_pixels[i][j].b) / 3 > threshold) {
                if (invert == false) {
                    this->m_pixels[i][j] = false;
                }
                else {
                    this->m_pixels[i][j] = true;
                }
                
            }
            else {
                if (invert == false) {
                    this->m_pixels[i][j] = true;
                }
                else {
                    this->m_pixels[i][j] = false;
                }
            }
        }
    }
}

void BMP::genBoolImage() {
    this->m_pixels.resize(this->height);
    for (int i = 0; i < this->height; i++) {
        this->m_pixels.at(i).resize(this->width);
    }
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->pixels[i][j].r == 0 && this->pixels[i][j].g == 0 && this->pixels[i][j].b == 0) {
                this->m_pixels[i][j] = true;
            }
            else {
                this->m_pixels[i][j] = false;
            }
        }
    }
}

void BMP::genPixels(bool invert) {
    for (int i = 0; i < this->height; i++) {
        for (int j = 0; j < this->width; j++) {
            if (this->m_pixels[i][j] == true) {
                if (invert == false) {
                    pixels[i][j] = { 0, 0, 0 };
                }
                else {
                    pixels[i][j] = { 255, 255, 255 };
                }
            }
            else {
                if (invert == false) {
                    pixels[i][j] = { 255, 255, 255 };
                }
                else {
                    pixels[i][j] = { 0, 0, 0 };
                }
            }
        }
    }
}
