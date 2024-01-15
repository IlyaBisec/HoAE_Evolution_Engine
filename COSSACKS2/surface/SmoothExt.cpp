#include "SmoothExt.h"

// SmoothExt::OnLMB
DWORD SmoothExt::OnLMB(SelExtParams &Params) {
	if(!Params.RefTo.m_TriNums.NValues) return 0;
	Params.RefTo.Relax();
	return IF_GEOMETRY;
} // SmoothExt::OnLMB

// SmoothExt::OnDraw
void SmoothExt::OnDraw(SelExtParams &Params) {
	DrawGrid(Params.RefTo, comms::argbCyan);
} // SmoothExt::OnDraw