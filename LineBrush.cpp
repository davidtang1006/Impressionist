//
// LineBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "linebrush.h"

extern float frand();

LineBrush::LineBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void LineBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void LineBrush::BrushMove(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("LineBrush::BrushMove document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	int lineWidth = dlg->getLineWidth();
	int lineAngle = dlg->getLineAngle();
	float alpha = dlg->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslatef(target.x, target.y, 0);
	glRotatef(lineAngle, 0, 0, 1);
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

void LineBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
