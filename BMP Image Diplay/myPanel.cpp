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
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	if (bmpFile->IsOpened()) {
		if (bmpFile->readMetaData()) {
			resizeToImage();
			maxWidth = GetSize().x;
			maxHeight = GetSize().y;
			bmpFile->readImageData();
		}
	}
	else {
		wxMessageBox("Error: Selected file not open for reading.");
	}
}

myPanel::~myPanel()
{
	delete bmpFile; //required to delete manually allocated elements of myBMPFile
}

void myPanel::resizeToImage()
{
	SetSize(bmpFile->getImageSize());
	maxWidth = GetSize().x;
	maxHeight = GetSize().y;
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
	wxPen pen = dc.GetPen();
	for (int row = 0; row < maxHeight; row++) {
		for (int col = 0; col < maxWidth; col++) {
			pen.SetColour(bmpFile->getPixelColour((maxWidth * row) + col));
			dc.SetPen(pen);
			dc.DrawPoint(col, maxHeight - row - 1);
		}
	}
	return;
}

myBMPFile* myPanel::getFile()
{
	return bmpFile;
}