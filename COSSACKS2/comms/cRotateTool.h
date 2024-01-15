#pragma once

//*****************************************************************************
// RotateTool
//*****************************************************************************

class cRotateTool : public cWidget {
public:
	cRotateTool();
	enum SPACE { SPACE_WORLD, SPACE_OBJECT };
	void Bind(cQuat *pRotate, cVec3 *pTranslate, const SPACE Space = SPACE_WORLD);
	void SetSpace(const SPACE Space);
	
	bool OnButtonDown(const int Code);
	bool OnButtonUp(const int Code);
	bool OnMouseMove(const int DeltaX, const int DeltaY);
	void OnRender();

private:
	// Size:
	float m_OuterRadius; // In pixels.
	float m_InnerRadius; // Fraction of outer radius.
	float m_MaxSelDistance; // Fraction of outer radius.
	
	cQuat *m_pRotate;
	cVec3 *m_pTranslate;

	SPACE m_Space, m_DesiredSpace;
	
	enum MODE { MODE_NONE, MODE_FREE, MODE_AROUND_X, MODE_AROUND_Y, MODE_AROUND_Z, MODE_AROUND_VIEW };
	MODE m_Mode;

	// Rotate Tool Frame (in World space):
	bool CalcFrame();
	struct tFrame {
		cVec3 Pos;
		// 0 - X, 1 - Y, 2 - Z, 3 - View
		cVec3 AxisNormal[4];
		float OuterRadius;
		float InnerRadius;
		float MaxSelDistance;

		cVec3 StartNormal;
		cQuat StartRotation;
		cVec3 CurNormal;
		cVec3 RotationAxis;
		float Angle;
	} m_Frame;
};