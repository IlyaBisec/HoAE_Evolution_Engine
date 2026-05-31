#pragma once

template <class Type>
class cRect {
public:
	cRect() {}
	inline cRect(const Type xLeft, const Type yTop, const Type xRight, const Type yBottom)
		: m_xLeft(xLeft), m_yTop(yTop), m_xRight(xRight), m_yBottom(yBottom) {}
	void Set(const Type xLeft, const Type yTop, const Type xRight, const Type yBottom) {
		m_xLeft = xLeft;
		m_yTop = yTop;
		m_xRight = xRight;
		m_yBottom = yBottom;
	}

	Type GetLeft() const { return m_xLeft; }
	Type GetRight() const { return m_xRight; }
	Type GetTop() const { return m_yTop; }
	Type GetBottom() const { return m_yBottom; }
	
	Type GetWidth() const { return m_xRight - m_xLeft; }
	Type GetHeight() const { return m_yBottom - m_yTop; }

private:
	Type m_xLeft;
	Type m_yTop;
	Type m_xRight;
	Type m_yBottom;
};

typedef cRect<int> cRectI;
typedef cRect<float> cRectF;