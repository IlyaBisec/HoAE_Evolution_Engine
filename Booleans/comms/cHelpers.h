#ifndef __cHelpers__
#define __cHelpers__

#include "cUtils.h"

namespace comms {

class cHelpers {
public:
	static BOOL CenterWindow(HWND hwndChild, HWND hwndParent);

	struct ExecArgs {
		const char *pVerb; 	// "edit", "explore", "find", "open", "print", "properties"
		const char *pFileName;
		const char *pParameters;
		const char *pDirectory;
		int nShow;
		bool fWait;
		DWORD msTimeOut;

		ExecArgs(const char *pVerb, const char *pFileName, const char *pParameters,
			const char *pDirectory, int nShow, bool fWait, DWORD msTimeOut = 1000) {
				this->pVerb = pVerb;
				this->pFileName = pFileName;
				this->pParameters = pParameters;
				this->pDirectory = pDirectory;
				this->nShow = nShow;
				this->fWait = fWait;
				this->msTimeOut = msTimeOut;
			}
	};
	static int Exec(const ExecArgs &Args);
};

} // comms

#endif // __cHelpers__