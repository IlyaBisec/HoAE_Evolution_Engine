#ifndef __CmdCallbacks__
#define __CmdCallbacks__

#include "..\surface\Surface.h"

namespace Surface {
	bool SimplifyCallback(cStr &Msg);
	bool GenerateCallback(cStr &Msg);
	bool RefreshCallback(cStr &Msg);

} // Surface

#endif // __CmdCallbacks__