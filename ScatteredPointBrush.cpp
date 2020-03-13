//
// ScatteredPointBrush.cpp
//

#include "impressionistDoc.h"
#include "impressionistUI.h"
#include "ScatteredPointBrush.h"

extern float frand();

ScatteredPointBrush::ScatteredPointBrush(ImpressionistDoc* pDoc, char* name) : ImpBrush(pDoc, name)
{
}

void ScatteredPointBrush::BrushBegin(const Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	BrushMove(source, target);
}

void ScatteredPointBrush::BrushMove(Point source, const Point target)
{
	ImpressionistDoc* pDoc = GetDocument();
	ImpressionistUI* dlg = pDoc->m_pUI;

	if (pDoc == NULL) {
		printf("ScatteredPointBrush::BrushMove document is NULL\n");
		return;
	}

	float alpha = dlg->getAlpha();
	int size = pDoc->getSize();
	float half = (float)size / 2.0f;

	glPointSize(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (frand() < 0.25f) {
				glPushMatrix();
				glTranslatef(-half + i, -half + j, 0);
				glBegin(GL_POINTS);

				source.translate(-(int)half + i, -(int)half + j);
				SetColor(source, alpha);
				source.translate((int)half - i, (int)half - j); // Revert the translation
				glVertex2d(target.x, target.y);

				glEnd();
				glPopMatrix();
			}
		}
	}
}

void ScatteredPointBrush::BrushEnd(const Point source, const Point target)
{
	// do nothing so far
}
