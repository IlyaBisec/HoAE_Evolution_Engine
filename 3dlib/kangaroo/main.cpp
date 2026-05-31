#include "stdafx.h"
#include "resource.h"
#include "sgRoot.h"
#include "uiControl.h"
#include "edUtilities.h"
#include "dbDebugger.h"
#include "ISpriteManager.h"
#include "IEffectManager.h"
#include "tablet.h"
#include "IResourceManager.h"


//#define __LSHEAD

#ifdef __LSHEAD
	#ifdef _INLINES
	#pragma comment (lib, "gHeadR.lib" )
	#else
	#pragma comment (lib, "gHead.lib" )
	#endif _INLINES
void LSHeadLinkStub();
#endif // __LSHEAD

bool LightmapLinkStub();
SNode* CreatePhysicsTemplates();

void LinkTemp();
void Render()
{
	Root::instance()->Render();
}


struct Poly
{
	WORD		v1;
	WORD		v2;
	WORD		v3;
}; // struct Poly

int __cdecl cmpIdxProximity( const void *e1, const void *e2 )
{
	const Poly& p1 = *((Poly*)e1);
	const Poly& p2 = *((Poly*)e2);

	return tmin(p1.v1, p1.v2, p1.v3) - tmin(p2.v1, p2.v2, p2.v3);

} // cmpIdxProximity

void LinkTemp();

HWND g_hSplashDialog = NULL;

void DestroySplashDialog(){
	static int T=GetTickCount();
	if(GetTickCount()-T>1000){
		if(g_hSplashDialog){
			DestroyWindow(g_hSplashDialog);
			g_hSplashDialog=NULL;
		}
	}
}


int APIENTRY WinMain(	HINSTANCE hInstance,
						HINSTANCE hPrevInstance,
						LPTSTR    lpCmdLine,
						int       nCmdShow )
{
	g_hSplashDialog = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_SPLASH), NULL, NULL);
	_chdir(IRM->GetHomeDirectory());

	HMODULE hHeadLib = LoadLibrary( "gHead.dll" );
	//g_WavPlayer.Init( AppWindow::instance()->GetWndHandle() );    
    //LinkTemp();

    static float x=0.1;
    float u=floorf(u);


    IRS->Init( hInstance );
    ISM->Init();    
	InitWinTab(IRS->GetHWND());
	Root::instance()->CreateGuts();
    extern bool g_bKangaroo;
    g_bKangaroo = true;

#ifdef __LSHEAD
	LSHeadLinkStub();
#endif // __LSHEAD
	
    Root::instance()->AddChild<Utilities>( "Utilities" );
    Root::instance()->AddChild<Debugger>( "Debug" );
    IEffMgr->EnableSelection( true );

    IRS->SetTransparentTexOverride(3);

    static DWORD lightID = IMM->GetNodeID( "GameLight" );
    ILight* IL=IMM->GetLight(lightID);
    if (IL)
    {
        IL->SetDiffuse(0xFFA0A0A0);
        IL->SetAmbient(0xFF303030);
        IL->Render();
    }
	IRS->SetFog(0,1,100000,0,0);	
    
    bool bGotMsg;
    MSG  msg;
    msg.message = WM_NULL;
    PeekMessage( &msg, NULL, 0U, 0U, PM_NOREMOVE );

    while( WM_QUIT != msg.message )
    {
        bGotMsg = ( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) != 0 );
        if( bGotMsg )
        {
			bool GlobalProcessWidgets(MSG& msg);
			if(!GlobalProcessWidgets(msg)){
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
        }
        else
        {			
            if (IRS->StartFrame())
            {
				extern int Diplimit;
				Diplimit=100000;
                Root::instance()->Render();				
                IRS->EndFrame();				
				DestroySplashDialog();	
            }
            else Sleep( 100 );
        }
    }
    return (INT)msg.wParam;
    
	if (hHeadLib) FreeLibrary( hHeadLib );

#ifdef _DEBUG
	//_CrtDumpMemoryLeaks();
#endif // _DEBUG

	return 0;
} // WinMain
