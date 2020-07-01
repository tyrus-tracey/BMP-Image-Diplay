#include "myPanel.h"

//Bind paint events to this panel
wxBEGIN_EVENT_TABLE(myPanel, wxPanel)
EVT_PAINT(myPanel::paintEvent)
END_EVENT_TABLE()

//Using the base class constructor, create a panel and an associated myWaveFile as a child
//The parent/child relationship allows for multiple panels to open with each one 
//	having it's own waveform. However, currently this is not implemented.
myPanel::myPanel(wxFrame* parent, const wxString filepath)
	: wxPanel(parent, wxID_ANY, wxPoint(0, 0), parent->GetSize())
{
	bmpFile = new myBMPFile(filepath);
	if (bmpFile == NULL) {
		wxMessageBox("Error: Could not find associated BMP file.");
	}
	maxSize = parent->GetSize();
	SetSize(maxSize);
	maxHeight = maxSize.y;
	maxWidth = maxSize.x;
	SetBackgroundStyle(wxBG_STYLE_PAINT);
}

myPanel::~myPanel()
{
	delete bmpFile; //required to delete manually allocated elements of myBMPFile
}

// 'dc' is a device context (the screen) that is used for drawing on the panel
void myPanel::paintEvent(wxPaintEvent& event)
{
	wxBufferedPaintDC dc(this);
	PrepareDC(dc);
	drawImage(dc);
}

// Prepare a clean background to draw the waveform on.
void myPanel::drawImage(wxDC& dc)
{
	wxBrush brush = dc.GetBrush();	// Rectangle body 
	wxPen pen = dc.GetPen();		// Rectangle border
	pen.SetStyle(wxPENSTYLE_SOLID);
	pen.SetColour(wxColour(100, 150, 200));
	dc.SetPen(pen);
	dc.SetBrush(brush);

	if (bmpFile->readMetaData()) {
		wxMessageBox("Successful Read.");
	}

	return;
}

myBMPFile* myPanel::getFile()
{
	return bmpFile;
}