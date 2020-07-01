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

}

bool myBMPFile::readMetaData() {
    if (!IsOpened()) {
        wxMessageBox("Error: File could not be read.");
        return false;
    }
    if (!readFileHeader() | !readInfoHeader()) {
        return false;
    }
    return true;
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
