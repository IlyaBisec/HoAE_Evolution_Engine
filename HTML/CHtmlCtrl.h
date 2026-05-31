#pragma once

#include <afxext.h>
#include <afxadv.h>
#include <afxcmn.h>
#include <afxcview.h>
#include <afxhtml.h>
#include <atlsafe.h>
#include <mshtml.h>
#include <afxdhtml.h>
#include <afxocc.h>
#include <exdispid.h>

struct HTMLCMDMAP {
	LPCTSTR Name;
	UINT nID;
};

class CHtmlCtrl : public CHtmlView {
protected:
	HTMLCMDMAP *m_pCmdMap;
	BOOL m_fHideContextMenu;
public:
	CHtmlCtrl() : m_fHideContextMenu(FALSE), m_pCmdMap(NULL) {}
	~CHtmlCtrl() {}

	BOOL HideContextMenu() { return m_fHideContextMenu; }
	void HideContextMenu(BOOL f) { m_fHideContextMenu = f; }

	HRESULT SetHTML(LPCTSTR strHTML);
	void SetCmdMap(HTMLCMDMAP *pCmdMap) { m_pCmdMap = pCmdMap; }

	BOOL CreateFromStatic(UINT nID, CWnd *pParent);

	BOOL Create(const RECT &rc, CWnd *pParent, UINT nID,
		DWORD dwStyle = WS_CHILD | WS_VISIBLE, CCreateContext *pContext = NULL) {
			return CHtmlView::Create(NULL, NULL, dwStyle, rc, pParent, nID, pContext);
	}

	virtual BOOL PreTranslateMessage(MSG *pMsg);

	virtual void PostNcDestroy() {}

	afx_msg void OnDestroy();
	afx_msg int OnMouseActivate(CWnd *pDesktopWnd, UINT nHitTest, UINT msg);

	virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName,
		CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL * pbCancel);
	virtual void OnNavigateComplete2(LPCTSTR strURL);

	virtual void OnAppCmd(LPCTSTR lpszCmd);

	DECLARE_MESSAGE_MAP();
	DECLARE_DYNAMIC(CHtmlCtrl)
};

class CCreateHTMLImage : public CWnd {
protected:
	DECLARE_DYNCREATE(CCreateHTMLImage)
	DECLARE_EVENTSINK_MAP()
	enum eEnums {
		CHILDBROWSER = 100,
	};
public:
	CCreateHTMLImage() {}
	virtual ~CCreateHTMLImage() {}
	
	BOOL Create(CWnd *pParent);
	BOOL CreateImage(IHTMLDocument2 *pDoc, LPCTSTR szDestFilename, CSize srcSize, CSize outputSize);
	BOOL CreateImage(LPCTSTR szSrcFilename, LPCTSTR szDestFilename, CSize srcSize, CSize outputSize);
protected:
	CComPtr<IWebBrowser2> m_pBrowser;
	CWnd m_pBrowserWnd;
	
	virtual BOOL CreateControlSite(COleControlContainer *pContainer, COleControlSite **ppSite, UINT nID, REFCLSID clsid);
	virtual void DocumentComplete(LPDISPATCH pDisp, VARIANT *URL);
};

class CHTMLImageControlSite : public COleControlSite {
public:
	CHTMLImageControlSite(COleControlContainer *pCtrlCont);
protected:
	BEGIN_INTERFACE_PART(DocHostUIHandler, IDocHostUIHandler)
		STDMETHOD(ShowContextMenu)(DWORD, LPPOINT, LPUNKNOWN, LPDISPATCH);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO*);
		STDMETHOD(ShowUI)(DWORD, LPOLEINPLACEACTIVEOBJECT, LPOLECOMMANDTARGET, LPOLEINPLACEFRAME, LPOLEINPLACEUIWINDOW);
		STDMETHOD(HideUI)(void);
		STDMETHOD(UpdateUI)(void);
		STDMETHOD(EnableModeless)(BOOL);
		STDMETHOD(OnDocWindowActivate)(BOOL);
		STDMETHOD(OnFrameWindowActivate)(BOOL);
		STDMETHOD(ResizeBorder)(LPCRECT, LPOLEINPLACEUIWINDOW, BOOL);
		STDMETHOD(TranslateAccelerator)(LPMSG, const GUID*, DWORD);
		STDMETHOD(GetOptionKeyPath)(OLECHAR **, DWORD);
		STDMETHOD(GetDropTarget)(LPDROPTARGET, LPDROPTARGET*);
		STDMETHOD(GetExternal)(LPDISPATCH*);
		STDMETHOD(TranslateUrl)(DWORD, OLECHAR*, OLECHAR **);
		STDMETHOD(FilterDataObject)(LPDATAOBJECT , LPDATAOBJECT*);
	END_INTERFACE_PART(DocHostUIHandler)
	DECLARE_INTERFACE_MAP()
};