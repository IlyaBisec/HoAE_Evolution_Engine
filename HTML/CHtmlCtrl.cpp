#include <afxwin.h>
#include "CHtmlCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DECLARE_SMARTPTR(ifacename) typedef CComQIPtr<ifacename> SP##ifacename;
DECLARE_SMARTPTR(IHTMLDocument2)

#define HRCHECK(x) hr = x; if (!SUCCEEDED(hr)) { \
	TRACE(_T("hr=%p\n"),hr);\
	return hr;\
}

IMPLEMENT_DYNAMIC(CHtmlCtrl, CHtmlView)
BEGIN_MESSAGE_MAP(CHtmlCtrl, CHtmlView)
	ON_WM_DESTROY()
	ON_WM_MOUSEACTIVATE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

//-----------------------------------------------------------------------------
// CreateFromStatic
//-----------------------------------------------------------------------------
BOOL CHtmlCtrl::CreateFromStatic(UINT nID, CWnd *pParent) {
	CStatic wndStatic;
	if(!wndStatic.SubclassDlgItem(nID, pParent)) return FALSE;
	
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();

	return Create(rc, pParent, nID);
}

//-----------------------------------------------------------------------------
// OnDestroy
//-----------------------------------------------------------------------------
void CHtmlCtrl::OnDestroy() {
	m_pBrowserApp = NULL;
	CWnd::OnDestroy();
}

//-----------------------------------------------------------------------------
// OnMouseActivate
//-----------------------------------------------------------------------------
int CHtmlCtrl::OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT msg) {
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, msg);
}

//-----------------------------------------------------------------------------
// IsIEWindow
//-----------------------------------------------------------------------------
inline BOOL IsIEWindow(HWND hWnd) {
	static LPCSTR IEWNDCLASSNAME = "Internet Explorer_Server";
	char ClassName[32];
	GetClassName(hWnd, ClassName, sizeof(ClassName));
	return strcmp(ClassName, IEWNDCLASSNAME) == 0;
}

//-----------------------------------------------------------------------------
// PreTranslateMessage
//-----------------------------------------------------------------------------
BOOL CHtmlCtrl::PreTranslateMessage(MSG *pMsg) {
	if(m_fHideContextMenu) {
		switch (pMsg->message) {
			case WM_KEYDOWN:
				if(pMsg->wParam == VK_ESCAPE) {
					GetParent()->SendMessage(WM_COMMAND, ID_APP_EXIT);
					return TRUE;
				}
				return FALSE;
			// RMB:
			case WM_CONTEXTMENU:
			case WM_RBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONDBLCLK:
			// MMB:
//			case WM_MBUTTONUP:
//			case WM_MBUTTONDOWN:
//			case WM_MBUTTONDBLCLK:
			// LMB:
//			case WM_LBUTTONUP:
//			case WM_LBUTTONDOWN:
//			case WM_LBUTTONDBLCLK:
				// Lock context menu:
				return TRUE;
/*				if(IsIEWindow(pMsg->hwnd)) {
					if(pMsg->message == WM_RBUTTONUP)
						GetParent()->SendMessage(WM_CONTEXTMENU, pMsg->wParam, pMsg->lParam);
					return TRUE;
				}*/
		}
	}
	return CHtmlView::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// CHtmlCtrl::OnNavigateComplete2
//-----------------------------------------------------------------------------
void CHtmlCtrl::OnNavigateComplete2(LPCTSTR URL) {
	const char KEY_WORD[] = "cmd:";
	size_t len = _tcslen(KEY_WORD);
	if(_tcsnicmp(URL, KEY_WORD, len) == 0) {
		OnAppCmd(URL + len);
	}
}

//------------------------------------------------------------------------------------------------
// CHtmlCtrl::OnBeforeNavigate2
//------------------------------------------------------------------------------------------------
void CHtmlCtrl::OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName,
								  CByteArray &baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel) {
/*	const char KEY_WORD[] = "cmd:";
	size_t len = _tcslen(KEY_WORD);
	if(_tcsnicmp(lpszURL, KEY_WORD, len) == 0) {
		OnAppCmd(lpszURL + len);
		*pbCancel = TRUE;
	}*/
}

//-----------------------------------------------------------------------------
// CHtmlCtrl::OnAppCmd
//-----------------------------------------------------------------------------
void CHtmlCtrl::OnAppCmd(LPCTSTR lpszCmd) {
	if(m_pCmdMap) {
		for (int i = 0; m_pCmdMap[i].Name; i++) {
			if(_tcsicmp(lpszCmd, m_pCmdMap[i].Name) == 0)
			GetParent()->PostMessage(WM_COMMAND, m_pCmdMap[i].nID);
		}
	}
}

//-----------------------------------------------------------------------------
// CHtmlCtrl::SetHTML
//-----------------------------------------------------------------------------
HRESULT CHtmlCtrl::SetHTML(LPCTSTR strHTML) {
	HRESULT hr;

	SPIHTMLDocument2 doc = GetHtmlDocument();

	CComSafeArray<VARIANT> sar;
	sar.Create(1, 0);
	sar[0] = CComBSTR(strHTML);

	LPDISPATCH lpdRet;
	HRCHECK(doc->open(CComBSTR("text/html"), CComVariant(CComBSTR("_self")), CComVariant(CComBSTR("")),
		CComVariant((bool)1), &lpdRet));
	
	HRCHECK(doc->write(sar));
	HRCHECK(doc->close());
	lpdRet->Release();
	return S_OK;
}

#ifdef _DEBUG
IMPLEMENT_DYNCREATE(CCreateHTMLImage, CWnd)
#endif

BEGIN_EVENTSINK_MAP(CCreateHTMLImage, CWnd)
	ON_EVENT(CCreateHTMLImage, CHILDBROWSER, DISPID_DOCUMENTCOMPLETE, DocumentComplete, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

BOOL CCreateHTMLImage::CreateControlSite(COleControlContainer* pContainer,  COleControlSite** ppSite, UINT, REFCLSID) {
	ASSERT(ppSite != NULL);
	*ppSite = new CHTMLImageControlSite(pContainer);
	return TRUE;
}

BOOL CCreateHTMLImage::Create(CWnd *pParent) {
	ASSERT(GetSafeHwnd() == HWND(NULL));
	ASSERT(pParent);
	ASSERT_KINDOF(CWnd, pParent);
	ASSERT(IsWindow(pParent->GetSafeHwnd()));

	CRect rect;

	if(!CWnd::Create(NULL, NULL, WS_CHILD, rect, pParent, 0, NULL)) return FALSE;
	if(!m_pBrowserWnd.CreateControl(CLSID_WebBrowser, _T(""), WS_VISIBLE | WS_CHILD, rect, this, CHILDBROWSER)) {
		DestroyWindow();
		return FALSE;
	}

	LPUNKNOWN lpUnk = m_pBrowserWnd.GetControlUnknown();
	HRESULT hr = lpUnk->QueryInterface(IID_IWebBrowser2, (void**) &m_pBrowser);

	if(!SUCCEEDED(hr)) {
		m_pBrowser = NULL;
		m_pBrowserWnd.DestroyWindow();
		DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CCreateHTMLImage::CreateImage(LPCTSTR szSrcFilename, LPCTSTR szDestFilename, CSize srcSize, CSize outputSize) {
	ASSERT(GetSafeHwnd());
	ASSERT(IsWindow(GetSafeHwnd()));
	ASSERT(szSrcFilename);
	ASSERT(AfxIsValidString(szSrcFilename));
	ASSERT(szDestFilename);
	ASSERT(AfxIsValidString(szDestFilename));

	CRect rect(CPoint(0, 0), srcSize);

	MoveWindow(&rect);
	m_pBrowserWnd.MoveWindow(&rect);
	
	COleVariant vUrl(szSrcFilename, VT_BSTR),
		vFlags(long(navNoHistory | navNoReadFromCache | navNoWriteToCache), VT_I4), vNull(LPCTSTR(NULL), VT_BSTR);
	COleSafeArray vPostData;

	if(m_pBrowser->Navigate2(&vUrl, &vFlags, &vNull, &vPostData, &vNull) == S_OK) RunModalLoop();
	else return FALSE;

	IDispatch *pDoc = (IDispatch *)NULL;
	HRESULT hr = m_pBrowser->get_Document(&pDoc);
	
	if (FAILED(hr)) return FALSE;
	return CreateImage((IHTMLDocument2 *)pDoc, szDestFilename, srcSize, outputSize);
}

class CBitmapDC : public CDC {
public:
	CBitmapDC(int nWidth, int nHeight, CDC *pSrcDC = NULL, COLORREF BkColor = RGB(255,255,255)) : CDC(), m_pBmp(NULL), m_pOldBmp(NULL) {
		CWindowDC ScreenDC(NULL);
		if(!pSrcDC) pSrcDC = &ScreenDC;
		m_pBmp = new CBitmap();
		VERIFY(m_pBmp->CreateCompatibleBitmap(pSrcDC, nWidth, nHeight));
		VERIFY(CreateCompatibleDC(pSrcDC));
		m_pOldBmp = SelectObject(m_pBmp);
		CDC::SetMapMode(pSrcDC->GetMapMode());
		FillSolidRect(0, 0, nWidth, nHeight, BkColor);
	}
	virtual ~CBitmapDC() {
		if(m_pBmp) {
			CBitmap *pBmp;
			pBmp = Close();
			delete pBmp;
			pBmp = NULL;
		}
	}
	CBitmap *Close() {
		ASSERT(m_pBmp);
		CBitmap *pBmp = m_pBmp;
		SelectObject(m_pOldBmp);
		m_pBmp = NULL;
		return pBmp;
	}
private:
  CBitmap *m_pBmp;
  CBitmap *m_pOldBmp;
};

PBITMAPINFO CreateBitmapInfo(HBITMAP hBmp) { 
    BITMAP bmp; 
    PBITMAPINFO pbmi; 
    WORD    cClrBits; 

	GetObject(hBmp, sizeof(BITMAP), (LPSTR)&bmp);

    cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel); 
    if(cClrBits == 1) cClrBits = 1; 
    else 
		if(cClrBits <= 4) cClrBits = 4; 
		else 
			if(cClrBits <= 8) cClrBits = 8; 
			else 
				if (cClrBits <= 16) cClrBits = 16; 
				else 
					if(cClrBits <= 24) cClrBits = 24; 
				    else cClrBits = 32; 

	if(cClrBits != 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + 
                    sizeof(RGBQUAD) * (1<< cClrBits));
	else 
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER)); 

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER); 
    pbmi->bmiHeader.biWidth = bmp.bmWidth; 
    pbmi->bmiHeader.biHeight = bmp.bmHeight; 
    pbmi->bmiHeader.biPlanes = bmp.bmPlanes; 
    pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel; 
    if(cClrBits < 24) 
		pbmi->bmiHeader.biClrUsed = (1<<cClrBits); 

	pbmi->bmiHeader.biCompression = BI_RGB; 

    pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8
                                  * pbmi->bmiHeader.biHeight; 
	pbmi->bmiHeader.biClrImportant = 0; 
    return pbmi; 
 } 


void SaveShot(LPTSTR pszFile, PBITMAPINFO pbmi, HBITMAP hBMP, HDC hDC) { 
	HANDLE hf;
	BITMAPFILEHEADER hdr;
	PBITMAPINFOHEADER pbih;
	LPBYTE lpBits;
	DWORD dwTotal;
	DWORD cb;
	BYTE *hp;
	DWORD dwTmp; 

    pbih = (PBITMAPINFOHEADER) pbmi; 
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

    GetDIBits(hDC, hBMP, 0, (WORD) pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS);
    
    hf = CreateFile(pszFile, GENERIC_READ | GENERIC_WRITE, 
                   (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE) NULL); 
    if (hf == INVALID_HANDLE_VALUE) return;
	
	hdr.bfType = 0x4d42;
    hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed 
                 * sizeof(RGBQUAD) + pbih->biSizeImage); 
    hdr.bfReserved1 = 0; 
    hdr.bfReserved2 = 0; 

    hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed 
                    * sizeof (RGBQUAD); 

	WriteFile(hf, (LPVOID) &hdr, sizeof(BITMAPFILEHEADER), (LPDWORD) &dwTmp,  NULL);
	WriteFile(hf, (LPVOID) pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof (RGBQUAD),
                  (LPDWORD) &dwTmp, ( NULL));

    dwTotal = cb = pbih->biSizeImage; 
    hp = lpBits; 
	WriteFile(hf, (LPSTR) hp, (int) cb, (LPDWORD) &dwTmp,NULL);

	CloseHandle(hf);
    GlobalFree((HGLOBAL)lpBits);
}


BOOL CCreateHTMLImage::CreateImage(IHTMLDocument2 *pDoc, LPCTSTR szDestFilename, CSize srcSize, CSize outputSize) {
	USES_CONVERSION;
	ASSERT(szDestFilename);
	ASSERT(AfxIsValidString(szDestFilename));
	ASSERT(pDoc);

	IHTMLElement *pElement = (IHTMLElement *)NULL;
	IHTMLElementRender *pRender = (IHTMLElementRender *)NULL;

	pDoc->get_body(&pElement);

	if(pElement == (IHTMLElement *)NULL) return FALSE;
	pElement->QueryInterface(IID_IHTMLElementRender, (void **)&pRender);
	if(pRender == (IHTMLElementRender *)NULL) return FALSE;

	CBitmapDC hDC(srcSize.cx, srcSize.cy);
	pRender->DrawToDC(hDC);
	CBitmap *pBM = hDC.Close();

	SaveShot("1.bmp", CreateBitmapInfo(*pBM), *pBM, hDC);

	delete pBM;

//	CFileSpec fsDest(szDestFilename);
//	CBitmapDC destDC(srcSize.cx, srcSize.cy);
	
//	pRender->DrawToDC(destDC);
//	CBitmap *pBM = destDC.Close();
	
//	Bitmap *gdiBMP = Bitmap::FromHBITMAP(HBITMAP(pBM->GetSafeHandle()), NULL);
//	Image *gdiThumb = gdiBMP->GetThumbnailImage(outputSize.cx, outputSize.cy);
	
//	gdiThumb->Save(T2W(fsDest.GetFullSpec()), &m_encoderClsid);
//	delete gdiBMP;
//	delete gdiThumb;
//	delete pBM;
	return TRUE;
}

void CCreateHTMLImage::DocumentComplete(LPDISPATCH pDisp, VARIANT *URL) {
	UNUSED_ALWAYS(pDisp);
	UNUSED_ALWAYS(URL);
	TRACE("CCreateHTMLImage::DocumentComplete\n");
	EndModalLoop(0);
}

BEGIN_INTERFACE_MAP(CHTMLImageControlSite, COleControlSite)
	INTERFACE_PART(CHTMLImageControlSite, IID_IDocHostUIHandler, DocHostUIHandler)
END_INTERFACE_MAP()

CHTMLImageControlSite::CHTMLImageControlSite(COleControlContainer* pContainer) : COleControlSite(pContainer) {}

STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::QueryInterface(REFIID riid, void **ppvObj) {
	METHOD_PROLOGUE_EX_(CHTMLImageControlSite, DocHostUIHandler)
	return pThis->ExternalQueryInterface(&riid, ppvObj);
}

STDMETHODIMP_(ULONG) CHTMLImageControlSite::XDocHostUIHandler::AddRef() {
	METHOD_PROLOGUE_EX_(CHTMLImageControlSite, DocHostUIHandler)
	return pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CHTMLImageControlSite::XDocHostUIHandler::Release() {
	METHOD_PROLOGUE_EX_(CHTMLImageControlSite, DocHostUIHandler)
	return pThis->ExternalRelease();
}

STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::ShowContextMenu(DWORD, POINT *, IUnknown *, IDispatch *) {
	return S_FALSE;
}

STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::GetHostInfo(DOCHOSTUIINFO *pInfo) {
	pInfo->dwFlags = DOCHOSTUIFLAG_NO3DBORDER | DOCHOSTUIFLAG_SCROLL_NO;
	return S_OK;
}

STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::ShowUI(DWORD, IOleInPlaceActiveObject *, IOleCommandTarget *, IOleInPlaceFrame * /*pFrame*/, IOleInPlaceUIWindow *) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::HideUI(void) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::UpdateUI(void) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::EnableModeless(BOOL) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::OnDocWindowActivate(BOOL) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::OnFrameWindowActivate(BOOL) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::ResizeBorder(LPCRECT, IOleInPlaceUIWindow *, BOOL) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::TranslateAccelerator(LPMSG, const GUID *, DWORD) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::GetOptionKeyPath(LPOLESTR *, DWORD) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::GetDropTarget(IDropTarget *, IDropTarget **) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::GetExternal(IDispatch **) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::TranslateUrl(DWORD, OLECHAR *, OLECHAR **) { return S_FALSE; }
STDMETHODIMP CHTMLImageControlSite::XDocHostUIHandler::FilterDataObject(IDataObject *, IDataObject **) { return S_FALSE; }