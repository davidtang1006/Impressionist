//
// ImpBrush.cpp
//
// The implementation of virtual brush. All the other brushes inherit from it.
//

#include <limits.h>
#include <math.h>

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ImpBrush.h"

// Static class member initializations
int ImpBrush::c_nBrushCount = 0;
ImpBrush** ImpBrush::c_pBrushes = NULL;

ImpBrush::ImpBrush(ImpressionistDoc* pDoc, char* name) : m_pDoc(pDoc), m_pBrushName(name)
{
}

void ImpBrush::CaptureDirectionBegin(const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	m_DirectionStartPoint = target;
}

void ImpBrush::CaptureDirectionEnd(const Point target) {
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	double x = pow((double)target.x - (double)m_DirectionStartPoint.x, 2);
	double y = pow((double)target.y - (double)m_DirectionStartPoint.y, 2);
	dlg->setSize(sqrt(x + y));

	x = (double)target.x - (double)m_DirectionStartPoint.x;
	y = (double)target.y - (double)m_DirectionStartPoint.y;
	double lineAngle = (atan2(y, x) < 0) ? atan2(y, x) * 180 / M_PI + 360 : atan2(y, x) * 180 / M_PI;
	dlg->setLineAngle((int)lineAngle);
}

//---------------------------------------------------
// Return m_pDoc, which connects the UI and brushes
//---------------------------------------------------
ImpressionistDoc* ImpBrush::GetDocument(void)
{
	return m_pDoc;
}

//---------------------------------------------------
// Return the name of the current brush
//---------------------------------------------------
char* ImpBrush::BrushName(void)
{
	return m_pBrushName;
}

//----------------------------------------------------
// Set the color to paint with to the color at source,
// which is the coord at the original window to sample
// the color from
//----------------------------------------------------
void ImpBrush::SetColor(const Point source, const float alpha)
{
	ImpressionistDoc* pDoc = GetDocument();

	GLubyte color[4];

	color[0] = pDoc->GetOriginalPixel(source)[0];
	color[1] = pDoc->GetOriginalPixel(source)[1];
	color[2] = pDoc->GetOriginalPixel(source)[2];
	color[3] = alpha * UCHAR_MAX;

	glColor4ub(color[0], color[1], color[2], color[3]);
}