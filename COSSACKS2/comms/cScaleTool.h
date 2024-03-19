#pragma once

//*****************************************************************************
// ScaleTool
//*****************************************************************************

class cScaleTool : public cWidget {
public:
	cScaleTool();
	void Bind(cVec3 *pScale, cQuat *pRotate, cVec3 *pTranslate);
	bool OnButtonDown(const int Code);
	bool OnButtonUp(const int Code);
	bool OnMouseMove(const int DeltaX, const int DeltaY);
	void OnRender();

private:
	// Size:
	float m_LineLen; // In pixels.
	float m_MaxSelDistance; // Fraction of line len.

	// Axis Cull:
	float m_MaxVisibleDot;
	
	cVec3 *m_pScale;
	cQuat *m_pRotate;
	cVec3 *m_pTranslate;
	
	enum MODE { MODE_NONE, MODE_ALONG_X, MODE_ALONG_Y, MODE_ALONG_Z, MODE_UNIFORM };
	MODE m_Mode;
	
	// Scale Tool Frame (in World space):
	bool CalcFrame();
	struct tFrame {
		// 0 - X, 1 - Y, 2 - Z, 3 - Central
		cVec3 AxisNormal[4];
		cVec3 HandlePos[4];
		float LineLen;
		float MaxSelDistance;
		cSeg PickRay;
		bool AxisIsVisible[4];

		cVec3 AbsScale;
		cVec3 RelScale;
		cVec3 PickedPoint;
	} m_Frame;
};