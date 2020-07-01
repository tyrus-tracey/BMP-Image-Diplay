#include "myBMPFile.h"

myBMPFile::myBMPFile(const wxString filepath)
    : wxFFile(filepath, "rb") {

}

myBMPFile::~myBMPFile() {

}