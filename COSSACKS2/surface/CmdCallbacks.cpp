#include "stdheader.h"
#include "CmdCallbacks.h"

namespace Surface {

//-----------------------------------------------------------------------------
// SimplifyCallback
//-----------------------------------------------------------------------------
bool SimplifyCallback(cStr &Msg) {
	static int nStage = 0;
	switch(nStage) {
		case 0:
			Msg = "Simplifying...";
			nStage = 1;
			return true;
		case 1:
			OptimizeSurfMesh(g_UniversalMap);
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.QUADS.FillMapOfHeights();
			Msg = "Simplified";
			nStage = 0;
			return false;
		default:
			assert(0);
	}
}

bool GeneratorOnePass(cStr &);
//-----------------------------------------------------------------------------
// GenerateCallback
//-----------------------------------------------------------------------------
bool GenerateCallback(cStr &Msg) {
	static int nStage = 0;
	switch(nStage) {
		case 0:
			assert(GeneratorOnePass(Msg));
			nStage = 1;
			return true;
		case 1:
			if(GeneratorOnePass(Msg)) return true;
			Msg = "Generated";
			nStage = 0;
			return false;
		default:
			assert(0);
	}
}

//-----------------------------------------------------------------------------
// RefreshCallback
//-----------------------------------------------------------------------------
void DirectPassMessage(cStr &Msg, const int nCurPass) {
	Msg.Free();
	Msg << "Direct Illumination" << " - " << nCurPass << "/" << g_SurfEditor.Shader.aQtyOfLights;
}
void LODsPassMessage(cStr &Msg, const int nCurPass, const int NPasses) {
	Msg.Free();
	Msg << "LODs" << " - " << nCurPass + 1 << "/" << NPasses;
}
bool RefreshCallback(cStr &Msg) {
	static int nStage = 0;
	static int nCurPass = 0;
	static int NPasses = 0;
	switch(nStage) {
		case 0:
			Msg = "Refreshing...";
			nStage = 1;
			return true;
		case 1:
			g_UniversalMap.QUADS.Compile();
			g_UniversalMap.QUADS.Init();
			g_UniversalMap.CalcNormals();
			g_UniversalMap.FillMapOfHeights();
			nStage = 4;
			switch(g_SurfEditor.Refresh.Illumination) {
				case I_LOCAL:
					Msg = "Local Illumination";
					nStage = 2;
					break;
				case I_DIRECT:
					nCurPass = 0;
					DirectPassMessage(Msg, nCurPass);
					nStage = 3;
					break;
			}
			return true;
		case 2: // Local Illumination
			g_UniversalMap.m_Shader.Shade(g_UniversalMap, I_LOCAL);
			nStage = 4;
			return true;
		case 3: // Direct Passes
			if(0 == nCurPass) {
				g_UniversalMap.m_Shader.BeginPassedDirectIllumination(g_UniversalMap);
				nCurPass = 1;
				DirectPassMessage(Msg, nCurPass);
			} else {
				if(!g_UniversalMap.m_Shader.OnePass()) { nStage = 4; }
				else {
					nCurPass++;
					DirectPassMessage(Msg, nCurPass);
				}
			}
			return true;
		case 4: // Try init LODs
			if(!g_SurfEditor.Refresh.LODs) {
				nStage = 6;
				return true;
			}
			NPasses = g_UniversalMap.QUADS.BeginPassedLODsGen();
			if(NPasses > 0) {
				nStage = 5;
				nCurPass = 0;
				LODsPassMessage(Msg, nCurPass, NPasses);
			} else {
				nStage = 6;
				return true;
			}
			return true;
		case 5:
			if(g_UniversalMap.QUADS.LODsOnePass()) {
				nCurPass++;
				LODsPassMessage(Msg, nCurPass, NPasses);
			} else {
				nStage = 6;
				return true;
			}
			return true;
		case 6:
			Msg = "Refreshed";
			nStage = 0;
			return false;
	}
}

} // Surface