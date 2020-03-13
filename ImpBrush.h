#ifndef IMPBRUSH_H
#define IMPBRUSH_H

//
// ImpBrush.h
//
// The header file of virtual brush. All the other brushes inherit from it.
//

#include <stdlib.h>

// Each brush type has an associated constant.
enum
{
	BRUSH_POINTS = 0,
	BRUSH_LINES,
	BRUSH_CIRCLES,
	BRUSH_SCATTERED_POINTS,
	BRUSH_SCATTERED_LINES,
	BRUSH_SCATTERED_CIRCLES,
	BRUSH_BOXES,
	BRUSH_CROSSES,
	BRUSH_TRIANGLES,
	BRUSH_SCATTERED_TRIANGLES,
	NUM_BRUSH_TYPE // Make sure this stays at the end!
};

enum class StrokeDirection
{
	SLIDER_OR_RIGHT_MOUSE = 0,
	GRADIENT,
	BRUSH_DIRECTION,
	NUM_STROKE_DIRECTION
};

class ImpressionistDoc; // Pre-declaring class

class Point
{
public:
	Point() {};
	Point(int xx, int yy) { x = xx; y = yy; };

	void translate(int x, int y) {
		this->x += x;
		this->y += y;
	}

	int x, y;
};

class ImpBrush
{
protected:
	ImpBrush::ImpBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

public:
	// The implementation of your brush should realize these virtual functions
	virtual void BrushBegin(const Point source, const Point target) = 0;
	virtual void BrushMove(const Point source, const Point target) = 0;
	virtual void BrushEnd(const Point source, const Point target) = 0;

	void CaptureDirectionBegin(const Point target);
	void CaptureDirectionMove(const Point target);
	void CaptureDirectionEnd(const Point target);

	void CaptureCursorDirectionBegin(const Point target);
	void CaptureCursorDirectionEnd(const Point target);

	void CaptureGradientDirection(const Point source);

	// according to the source image and the position, determine the draw color
	void SetColor(const Point source, const float alpha);

	// get Doc to communicate with it
	ImpressionistDoc* GetDocument(void);

	// Return the name of the brush (not used in this version).
	char* BrushName(void);

	static int c_nBrushCount; // How many brushes we have,
	static ImpBrush** c_pBrushes; // and what they are.

private:
	ImpressionistDoc* m_pDoc;

	// Brush's name (not used in this version).
	char* m_pBrushName;

	Point m_DirectionStartPoint;
	Point m_CursorPreviousLocation;
};

#endif