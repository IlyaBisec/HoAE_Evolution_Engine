#pragma once

#include "SelExt.h"

namespace Surface {
	
class Chamfer : public SelExt {
public:
	DWORD OnLMB(SelExtArgs &Args);
	void OnDraw(SelExtArgs &Args);
};

} // Surface