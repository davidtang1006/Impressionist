//
// TriangleBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "TriangleBrush.h"
#include <math.h>

extern float frand();

TriangleBrush::TriangleBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void TriangleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void TriangleBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("TriangleBrush::BrushMove document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	float alpha = dlg->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(-30, 0, 0, 1);
	glTranslatef(-target.x, -target.y, 0);
	glTranslatef(target.x, target.y, 0);
	glBegin(GL_POLYGON);

	SetColor(source, alpha);
	int circlePoints = 3;
	for (int i = 0; i < circlePoints; i++) {
		float angle = 2 * M_PI * i / circlePoints;
		glVertex2f(size * cos(angle), size * sin(angle));
	}

	glEnd();
	glPopMatrix();
}

void TriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
