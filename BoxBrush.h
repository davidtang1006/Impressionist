//
// BoxBrush.h
//

#ifndef BOXBRUSH_H
#define BOXBRUSH_H

#include "ImpBrush.h"

class BoxBrush : public ImpBrush
{
public:
	BoxBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif