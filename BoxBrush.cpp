//
// BoxBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "BoxBrush.h"
#include <math.h>

extern float frand();

BoxBrush::BoxBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void BoxBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void BoxBrush::BrushMove(Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("BoxBrush::BrushMove document is NULL\n");
		return;
	}

	float alpha = dlg->getAlpha();
	int size = pDoc->getSize();
	float half = (float)size / 2.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLineWidth((float)ceil((float)size / 8.0f));

	glPushMatrix();
	glTranslatef(-half, -half, 0);
	glBegin(GL_LINE_STRIP);

	SetColor(source, alpha);
	glVertex2d(target.x, target.y);
	glVertex2d(target.x + size, target.y);
	glVertex2d(target.x + size, target.y + size);
	glVertex2d(target.x, target.y + size);
	glVertex2d(target.x, target.y);

	glEnd();
	glPopMatrix();
}

void BoxBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
