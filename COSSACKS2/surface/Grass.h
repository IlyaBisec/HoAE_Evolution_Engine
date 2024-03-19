#pragma once

#include "SelExt.h"

namespace Surface {

	class Grass : public SelExt {
	public:
		DWORD OnLMB(SelExtArgs &Args);
		DWORD OnRMB(SelExtArgs &Args);
		void OnDraw(SelExtArgs &Args);
	};

} // Surface