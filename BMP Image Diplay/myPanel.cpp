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
			bmpFile->readImageData();
			image = *(bmpFile->pixelVector);
			displayStatus = NORMAL;
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
	wxSize test = bmpFile->getImageSize();
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
			pen.SetColour(getPixelColor((maxWidth * row) + col));
			dc.SetPen(pen);
			dc.DrawPoint(col, maxHeight - row - 1);
		}
	}
	wxString statusText = wxString::Format(wxT("%i"), displayStatus);
	dc.DrawRectangle(0, 0, 20, 20);
	dc.DrawText(statusText, wxPoint(10, 10));
	return;
}

void myPanel::loadNext()
{
	switch (displayStatus) {
		case NORMAL:
			loadGrayscale();
			displayStatus = GRAYSCALE;
			break;
		case GRAYSCALE:
			loadNormal();
			displayStatus = NORMAL;
			break;
		case DARK:
			break;
		case VIVID:
			loadNormal();
			displayStatus = NORMAL;
			break;
		default:
			displayStatus = NORMAL;
			break;
	}
}

void myPanel::loadNormal() {
	vector<wxColor>::iterator fileIndex = bmpFile->getPixelVector()->begin();
	vector<wxColor>::iterator panelIndex = image.begin();

	while (fileIndex != bmpFile->getPixelVector()->end()) {
		wxColor temp = *fileIndex; // <- BUG: is loadGrayscale writing to the BMP vector?
		*panelIndex = temp;
		fileIndex++, panelIndex++;
	}
}

void myPanel::loadGrayscale()
{
	vector<wxColor>::iterator fileIndex = bmpFile->getPixelVector()->begin();
	vector<wxColor>::iterator panelIndex = image.begin();

	while (fileIndex != bmpFile->getPixelVector()->end()) {
		wxColor temp = *fileIndex;
		int Luma = std::round((temp.Red() * 0.299) + (temp.Green() * 0.587) + (temp.Blue() * 0.114));
		*panelIndex = wxColor(Luma, Luma, Luma);
		fileIndex++, panelIndex++;
	}
}

void myPanel::loadDarker()
{
	vector<wxColor>::iterator fileIndex = bmpFile->getPixelVector()->begin();
	vector<wxColor>::iterator panelIndex = image.begin();

	while (fileIndex != bmpFile->getPixelVector()->end()) {
		wxColor temp = *fileIndex;
		int Luma = std::round((temp.Red() * 0.299) + (temp.Green() * 0.587) + (temp.Blue() * 0.114));
		Luma *= 1.5;
		//convert back to RGB from YCoCG
		*panelIndex = temp;
		fileIndex++, panelIndex++;
	}
}

void myPanel::loadVivid()
{
	vector<wxColor>::iterator fileIndex = bmpFile->getPixelVector()->begin();
	vector<wxColor>::iterator panelIndex = image.begin();

	while (fileIndex != bmpFile->getPixelVector()->end()) {
		wxColor temp = *fileIndex;
		//vivid opertion here
		*panelIndex = temp;
		fileIndex++, panelIndex++;
	}
}

wxColor myPanel::getPixelColor(const int index) const {
	if (index >= maxWidth * maxHeight) {
		return wxColor("Black");
	}
	return image.at(index);

}

myBMPFile* myPanel::getFile()
{
	return bmpFile;
}