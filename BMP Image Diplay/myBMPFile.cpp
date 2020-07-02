#include "myBMPFile.h"

myBMPFile::myBMPFile(const wxString filepath)
    : wxFFile(filepath, "rb") {
    fileSize = 0;
    imageSize = 0;
    pixelOffset = 0;
    headerSize = 0;
    imageWidth = 0;
    imageHeight = 0;
    numberOfPlanes = 0;
    bitsPerPixel = 0;
    compressionFactor = 0;
}

myBMPFile::~myBMPFile() {
    delete pixelArray;
}

wxSize myBMPFile::getImageSize()
{
    return wxSize(imageWidth, imageHeight);
}

wxColor myBMPFile::getPixelColour(const int index) const
{
    return pixelArray->at(index);
}

bool myBMPFile::readMetaData() {
    if (!readFileHeader() | !readInfoHeader()) {
        return false;
    }
    return true;
}

// Create a temporary vector of enough size to store pixel data
// Then allocate a color vector of exact pixel count and copy.
void myBMPFile::readImageData()
{
    //Calculate size of byte padding;
    unsigned int bytesPerRow = imageWidth * bitsPerPixel / 8;
    unsigned int bytePadding = bytesPerRow % 4;
    char *paddingBuffer = new char[bytePadding];

    int i = 0;
    unsigned int pixelCount = 0;
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
    pixelArray = new vector<wxColor>(fileSize/3);
    vector<wxColor>::iterator index = pixelArray->begin();
    while (!Eof() || index != pixelArray->end()) {
        Read(&Blue, 1);
        Read(&Green, 1);
        Read(&Red, 1);
        Read(paddingBuffer, bytePadding);
        *index = wxColor(Red, Green, Blue);
        index++, pixelCount++;
    }
    pixelArray->resize(pixelCount);
    delete[] paddingBuffer;
    Close();
}

bool myBMPFile::readFileHeader()
{
    char charBuff[3];
    Read(charBuff, 2);
    charBuff[2] = '\0';
    if (strcmp(charBuff, "BM")) {
        wxMessageBox("Error: Selected file is not a BMP image.");
        return false;
    }

    Read(&fileSize, 4);
    Read(charBuff, 2);  // Reserved spaces
    Read(charBuff, 2);  // not needed
    Read(&pixelOffset, 4);

    return true;
}

bool myBMPFile::readInfoHeader()
{
    int junkBuffer;

    Read(&headerSize, 4);
    if (headerSize != 40) {
        wxMessageBox("Error: Header size is not size 40.");
        return false;
    }

    Read(&imageWidth, 4);
    Read(&imageHeight, 4);
    Read(&numberOfPlanes, 2);
    Read(&bitsPerPixel, 2);
    Read(&compressionFactor, 4);
    Read(&imageSize, 4);
    Read(&junkBuffer, 4); // X px per meter - not necessary
    Read(&junkBuffer, 4); // Y px per meter - not necessary
    Read(&junkBuffer, 4); // Total colors - not used in 24bit BMP
    Read(&junkBuffer, 4); // Important colors - not used in 24bit BMP

    if (bitsPerPixel != 24) {
        wxMessageBox("Error: Unsupported bit depth.");
        return false;
    }
    if (compressionFactor != 0) {
        wxMessageBox("Error: Compressed images are not supported.");
        return false;
    }
    if (numberOfPlanes != 1) {
        wxMessageBox("Warning: Loading non-standard color planes.");
    }
    return true;
}
