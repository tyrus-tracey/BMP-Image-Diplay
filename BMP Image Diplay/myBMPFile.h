#pragma once
#include "wx/string.h"
#include "wx/wfstream.h"

class myBMPFile : public wxFFile
{
public:
	myBMPFile(const wxString filepath);
	~myBMPFile();
};

