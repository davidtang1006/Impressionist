//
// ScatteredTriangleBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredTriangleBrush.h"
#include <math.h>

extern float frand();

ScatteredTriangleBrush::ScatteredTriangleBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void ScatteredTriangleBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void ScatteredTriangleBrush::BrushMove(Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredTriangleBrush::BrushMove document is NULL\n");
		return;
	}

	int size = pDoc->getSize();
	float alpha = dlg->getAlpha();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < 3; i++) {
		float randomMovementX = (frand() - 0.5f) * 3 * size;
		float randomMovementY = (frand() - 0.5f) * 3 * size;

		glPushMatrix();
		glTranslatef(target.x, target.y, 0);
		glRotatef(-30, 0, 0, 1);
		glTranslatef(-target.x, -target.y, 0);
		glTranslatef(target.x + randomMovementX, target.y + randomMovementY, 0);
		glBegin(GL_POLYGON);

		source.translate(randomMovementX, randomMovementY);
		SetColor(source, alpha);
		source.translate(-randomMovementX, -randomMovementY); // Revert the translation
		int circlePoints = 3;
		for (int i = 0; i < circlePoints; i++) {
			float angle = 2 * M_PI * i / circlePoints;
			glVertex2f(size * cos(angle), size * sin(angle));
		}

		glEnd();
		glPopMatrix();
	}
}

void ScatteredTriangleBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
