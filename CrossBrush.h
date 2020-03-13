//
// CrossBrush.h
//

#ifndef CROSSBRUSH_H
#define CROSSBRUSH_H

#include "ImpBrush.h"

class CrossBrush : public ImpBrush
{
public:
	CrossBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);
};

#endif