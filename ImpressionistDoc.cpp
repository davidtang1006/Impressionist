//
// impressionistDoc.cpp
//
// It basically maintain the bitmap for answering the color query from the brush.
// It also acts as the bridge between brushes and UI (including views)
//

#include <FL/fl_ask.H>
#include <math.h>

#include "ImpBrush.h"
#include "impressionistDoc.h"
#include "impressionistUI.h"

// Include individual brush headers here.
#include "PointBrush.h"
#include "LineBrush.h"
#include "CircleBrush.h"
#include "ScatteredLineBrush.h"
#include "ScatteredPointBrush.h"
#include "ScatteredCircleBrush.h"
#include "BoxBrush.h"
#include "CrossBrush.h"
#include "TriangleBrush.h"
#include "ScatteredTriangleBrush.h"

#define DESTROY(p) { if ((p)!=NULL) {delete [] p; p=NULL; } }

ImpressionistDoc::ImpressionistDoc()
{
	// Set NULL image name as init.
	m_imageName[0] = '\0';

	m_nWidth = -1;
	m_ucBitmap = NULL;
	m_ucPainting = NULL;

	// create one instance of each brush
	ImpBrush::c_nBrushCount = NUM_BRUSH_TYPE;
	ImpBrush::c_pBrushes = new ImpBrush * [ImpBrush::c_nBrushCount];

	ImpBrush::c_pBrushes[BRUSH_POINTS] = new PointBrush(this, "Points");

	// Note: You should implement these 5 brushes. They are set the same (PointBrush) for now.
	ImpBrush::c_pBrushes[BRUSH_LINES]
		= new LineBrush(this, "Lines");
	ImpBrush::c_pBrushes[BRUSH_CIRCLES]
		= new CircleBrush(this, "Circles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_POINTS]
		= new ScatteredPointBrush(this, "Scattered Points");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_LINES]
		= new ScatteredLineBrush(this, "Scattered Lines");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_CIRCLES]
		= new ScatteredCircleBrush(this, "Scattered Circles");

	// Additional brushes
	ImpBrush::c_pBrushes[BRUSH_BOXES]
		= new BoxBrush(this, "Boxes");
	ImpBrush::c_pBrushes[BRUSH_CROSSES]
		= new CrossBrush(this, "Crosses");
	ImpBrush::c_pBrushes[BRUSH_TRIANGLES]
		= new TriangleBrush(this, "Triangles");
	ImpBrush::c_pBrushes[BRUSH_SCATTERED_TRIANGLES]
		= new ScatteredTriangleBrush(this, "Scattered Triangles");

	// Initialization
	m_pCurrentBrush = ImpBrush::c_pBrushes[0];
	m_pCurrentStrokeDirection = StrokeDirection::SLIDER_OR_RIGHT_MOUSE;
}

//---------------------------------------------------------
// Set the current UI
//---------------------------------------------------------
void ImpressionistDoc::setUI(ImpressionistUI* ui)
{
	m_pUI = ui;
}

//---------------------------------------------------------
// Returns the active picture/painting name
//---------------------------------------------------------
char* ImpressionistDoc::getImageName()
{
	return m_imageName;
}

//---------------------------------------------------------
// Called by the UI when the user changes the brush type.
// type: one of the defined brush types.
//---------------------------------------------------------
void ImpressionistDoc::setBrushType(int type)
{
	m_pCurrentBrush = ImpBrush::c_pBrushes[type];
}

void ImpressionistDoc::setStrokeDirectionType(int type)
{
	m_pCurrentStrokeDirection = (StrokeDirection)type;
}

//---------------------------------------------------------
// Returns the size of the brush.
//---------------------------------------------------------
int ImpressionistDoc::getSize()
{
	return m_pUI->getSize();
}

//---------------------------------------------------------
// Load the specified image
// This is called by the UI when the load image button is
// pressed.
//---------------------------------------------------------
int ImpressionistDoc::loadImage(char* iname)
{
	// try to open the image to read
	unsigned char* data;
	int width, height;

	if ((data = readBMP(iname, width, height)) == NULL)
	{
		fl_alert("Can't load bitmap file");
		return 0;
	}

	// reflect the fact of loading the new image
	m_nWidth = width;
	m_nPaintWidth = width;
	m_nHeight = height;
	m_nPaintHeight = height;

	// release old storage
	if (m_ucBitmap) delete[] m_ucBitmap;
	if (m_ucPainting) delete[] m_ucPainting;

	m_ucBitmap = data;

	// allocate space for draw view
	m_ucPainting = new unsigned char[width * height * 3];
	memset(m_ucPainting, 0, width * height * 3);

	m_pUI->m_mainWindow->resize(m_pUI->m_mainWindow->x(),
		m_pUI->m_mainWindow->y(),
		width * 2,
		height + 25);

	// display it on origView
	m_pUI->m_origView->resizeWindow(width, height);
	m_pUI->m_origView->refresh();

	// refresh paint view as well
	m_pUI->m_paintView->resizeWindow(width, height);
	m_pUI->m_paintView->refresh();

	return 1;
}

//----------------------------------------------------------------
// Save the specified image
// This is called by the UI when the save image menu button is
// pressed.
//----------------------------------------------------------------
int ImpressionistDoc::saveImage(char* iname)
{
	writeBMP(iname, m_nPaintWidth, m_nPaintHeight, m_ucPainting);

	return 1;
}

//----------------------------------------------------------------
// Clear the drawing canvas
// This is called by the UI when the clear canvas menu item is
// chosen
//-----------------------------------------------------------------
int ImpressionistDoc::clearCanvas()
{
	// Release old storage
	if (m_ucPainting)
	{
		delete[] m_ucPainting;

		// allocate space for draw view
		m_ucPainting = new unsigned char[m_nPaintWidth * m_nPaintHeight * 3];
		memset(m_ucPainting, 0, m_nPaintWidth * m_nPaintHeight * 3);

		// refresh paint view as well
		m_pUI->m_paintView->refresh();
	}

	return 0;
}

//------------------------------------------------------------------
// Get the color of the pixel in the original image at coord x and y
//------------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(int x, int y)
{
	if (x < 0)
		x = 0;
	else if (x >= m_nWidth)
		x = m_nWidth - 1;

	if (y < 0)
		y = 0;
	else if (y >= m_nHeight)
		y = m_nHeight - 1;

	return (GLubyte*)(m_ucBitmap + 3 * (y * m_nWidth + x));
}

//----------------------------------------------------------------
// Get the color of the pixel in the original image at point p
//----------------------------------------------------------------
GLubyte* ImpressionistDoc::GetOriginalPixel(const Point p)
{
	return GetOriginalPixel(p.x, p.y);
}

float ImpressionistDoc::calculateBlurredPixel(int x, int y, int rgb) {
	int sum = 0;
	for (int i = -1; i < 2; i++) {
		for (int j = -1; j < 2; j++) {
			sum += (int)GetOriginalPixel(x + i, y + j)[rgb];
		}
	}
	return (float)sum / 9.0f;
}

float calculateIntensity(float r, float g, float b)
{
	return r * 0.299f + g * 0.587f + b * 0.144f;
}

int ImpressionistDoc::GetAnglePerpendicularToGradient(const Point source)
{
	float colorX1[3];
	float colorX2[3];
	float colorY1[3];
	float colorY2[3];

	float intensityX1;
	float intensityX2;
	float intensityY1;
	float intensityY2;

	colorX1[0] = calculateBlurredPixel(source.x, source.y, 0);
	colorX1[1] = calculateBlurredPixel(source.x, source.y, 1);
	colorX1[2] = calculateBlurredPixel(source.x, source.y, 2);
	intensityX1 = calculateIntensity(colorX1[0], colorX1[1], colorX1[2]);

	colorX2[0] = calculateBlurredPixel(source.x + 1, source.y, 0);
	colorX2[1] = calculateBlurredPixel(source.x + 1, source.y, 1);
	colorX2[2] = calculateBlurredPixel(source.x + 1, source.y, 2);
	intensityX2 = calculateIntensity(colorX2[0], colorX2[1], colorX2[2]);

	colorY1[0] = calculateBlurredPixel(source.x, source.y, 0);
	colorY1[1] = calculateBlurredPixel(source.x, source.y, 1);
	colorY1[2] = calculateBlurredPixel(source.x, source.y, 2);
	intensityY1 = calculateIntensity(colorY1[0], colorY1[1], colorY1[2]);

	colorY2[0] = calculateBlurredPixel(source.x, source.y + 1, 0);
	colorY2[1] = calculateBlurredPixel(source.x, source.y + 1, 1);
	colorY2[2] = calculateBlurredPixel(source.x, source.y + 1, 2);
	intensityY2 = calculateIntensity(colorY2[0], colorY2[1], colorY2[2]);

	float x = -intensityX1 + intensityX2;
	float y = -intensityY1 + intensityY2;

	double angle = (atan2(y, x) < 0) ? atan2(y, x) * 180 / M_PI + 360 : atan2(y, x) * 180 / M_PI;
	int result = ((int)angle + 90) % 360;

	return result;
}
