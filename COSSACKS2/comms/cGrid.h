#pragma once

//*****************************************************************************
// Grid
//*****************************************************************************

class cGrid : public cWidget {
public:
	cGrid();
	void SetDefaults();

	void OnRender();
private:
	// Size:
	float m_Size;				// >= 1.0f
	float m_UpAxisLength;		// >= Epsilon
	float m_GridLinesEvery;		// >= Epsilon
	int m_Subdivisions;			// >= 1
	// Appearance:
	bool m_ShowAxes;
	bool m_ShowGridLines;
	bool m_ShowSubdivisionLines;
};