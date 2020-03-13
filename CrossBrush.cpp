//
// CrossBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "CrossBrush.h"
#include <math.h>

extern float frand();

CrossBrush::CrossBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void CrossBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void CrossBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("CrossBrush::BrushMove document is NULL\n");
		return;
	}

	int size = pDoc->getSize() + 1;
	int lineWidth = (int)ceil((float)size / 4.0f) + 1;
	float alpha = dlg->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(45, 0, 0, 1);
	glTranslatef(-target.x, -target.y, 0);
	glBegin(GL_POLYGON);

	SetColor(source, alpha);
	glVertex2f(target.x - (float)size / 2.0f, target.y - (float)lineWidth / 2.0f);
	glVertex2f(target.x + (float)size / 2.0f, target.y - (float)lineWidth / 2.0f);
	glVertex2f(target.x + (float)size / 2.0f, target.y + (float)lineWidth / 2.0f);
	glVertex2f(target.x - (float)size / 2.0f, target.y + (float)lineWidth / 2.0f);

	glEnd();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(-45, 0, 0, 1);
	glTranslatef(-target.x, -target.y, 0);
	glBegin(GL_POLYGON);

	SetColor(source, alpha);
	glVertex2f(target.x - (float)size / 2.0f, target.y - (float)lineWidth / 2.0f);
	glVertex2f(target.x + (float)size / 2.0f, target.y - (float)lineWidth / 2.0f);
	glVertex2f(target.x + (float)size / 2.0f, target.y + (float)lineWidth / 2.0f);
	glVertex2f(target.x - (float)size / 2.0f, target.y + (float)lineWidth / 2.0f);

	glEnd();
	glPopMatrix();
}

void CrossBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
