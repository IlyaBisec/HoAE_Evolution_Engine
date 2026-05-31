#ifndef __SurfProcess__
#define __SurfProcess__

#include "Surface.h"
#include "SurfProcessResource.h"

namespace Surface {
	class SurfProcess {
	public:
		HWND m_hInputLabel, m_hOutputLabel, m_hToolCombo, m_hConsole, m_hDlg;
		cStr m_InputFileName, m_OutputFileName;
		bool m_fActive;
		SurfProcess() { m_fActive = false; }

		void Message(const cStr &Str);
		void AdjustLayout(HWND hDlg);
		void EnableLayout(bool fEnable);
		void InitLayout(HWND hDlg);
		void Show(HINSTANCE hInstance);
		static void InitDir(char *pStr);
		
		typedef enum { S_BEGIN, S_END, S_DECOMPRESS, S_COMPRESS, S_LOAD, S_SAVE, S_FINDFILE,
			S_FINDBLOCK, S_SEEKBLOCK, S_SAVEBLOCK, S_SEEKFILE, S_DELETEFILE, S_FINDSURFBLOCK } eStage;
		static bool ProcessStage(eStage nStage, void *pParam = NULL, void *pData = NULL, void *pAux = NULL);

		static const cStr csFileName;
		static const cStr csSize;
		static const cStr csDef;
		
		static int FileInfo(const cStr &FileName, cStr &Info);
		static DWORD Pack(const char *);
	};
} // Surface

extern Surface::SurfProcess g_SurfProcess;

#endif // __SurfProcess__