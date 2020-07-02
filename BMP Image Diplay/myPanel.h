#pragma once
#include "wx/wx.h"
#include "wx/dcbuffer.h"
#include "wx/panel.h"
#include "myBMPFile.h"

/*
	Panels are windows within frames that allow for drawing operations.
*/

class myPanel : public wxPanel
{
public:
	myPanel(wxFrame* parent, const wxString filepath);
	~myPanel();
	void resizeToImage();
	void paintEvent(wxPaintEvent& event);
	void drawImage(wxDC& dc);
	myBMPFile* getFile();

private:

	myBMPFile* bmpFile;
	wxSize maxSize;
	int	maxHeight;
	int maxWidth;

	DECLARE_EVENT_TABLE();
};

