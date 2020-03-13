//
// CircleBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CircleBrush.h"
#include <math.h>

extern float frand();

CircleBrush::CircleBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void CircleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CircleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CircleBrush::BrushMove document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	float alpha = dlg->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glBegin(GL_POLYGON);

	SetColor(source, alpha);
	int circlePoints = 100;
	for (int i = 0; i < circlePoints; i++) {
		float angle = 2 * M_PI * i / circlePoints;
		glVertex2f(size * cos(angle), size * sin(angle));
	}

	glEnd();
	glPopMatrix();
}

void CircleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
