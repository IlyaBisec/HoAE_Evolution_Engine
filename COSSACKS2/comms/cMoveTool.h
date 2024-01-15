#pragma once

//*****************************************************************************
// MoveTool
//*****************************************************************************

class cMoveTool : public cWidget {
public:
	cMoveTool();
	enum SPACE {
		SPACE_WORLD, SPACE_OBJECT
	};
	void Bind(cVec3 *pTranslate); // World space only.
	void Bind(cQuat *pRotate, cVec3 *pTranslate, const SPACE Space = SPACE_WORLD);
	void SetSpace(const SPACE Space); // Only when binded with quaternion.

	bool OnButtonDown(const int Code);
	bool OnButtonUp(const int Code);
	bool OnMouseMove(const int DeltaX, const int DeltaY);
	void OnRender();

private:
	enum MODE { MODE_NONE, MODE_ALONG_X, MODE_ALONG_Y, MODE_ALONG_Z, MODE_CENTER };
	MODE m_Mode;
	SPACE m_Space, m_DesiredSpace;

	// Size:
	float m_AxisLen; // In pixels.
	float m_MaxSelDistance; // Fraction of arrow len.

	// Axis Cull:
	float m_MaxVisibleDot;
	
	cQuat *m_pRotate;
	cVec3 *m_pTranslate;

	// Move Tool Frame (in World space):
	bool CalcFrame();
	struct tFrame {
		cVec3 CurPos;
		cSeg Axis[3];
		float MaxSelDistance;
		bool AxisIsVisible[3];

		cVec3 PickedPoint;
		cVec3 StartPos;
		cPlane Center;
		cSeg StartAxis[3];
	} m_Frame;
};