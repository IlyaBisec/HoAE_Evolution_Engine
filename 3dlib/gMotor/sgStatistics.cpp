#include "stdafx.h"
#include "sg.h"
#include "uiControl.h"
#include "kStatistics.h"
#include "kTimer.h"
#include "sgStatistics.h"
#include "IFontManager.h"
#include "IEffectManager.h"

IStatistics* IStats = NULL;

IMPLEMENT_CLASS( StatManager );

/*****************************************************************************/
/*    StatManager implementation
/*****************************************************************************/
StatManager::StatManager()
{
    IStats = this;
}

void StatManager::Render()
{
    static Timer s_Timer;
    float sec = s_Timer.seconds(); 
    s_Timer.start();

    float fps = 0.0f;
    if (sec > 0.0f) 
    { 
        fps = 1.0f / sec;
        m_FPS.push( fps );
    }

    Vector3D pos( 10, 10, 0.0f );
    char text[256];
    sprintf( text, "FPS: %.2f", fps );
    DrawText( pos.x, pos.y, 0xFFFF0000, text );
    FlushText();

    SNode::Render();
    Stats::OnFrame();
} // StatManager::Render



