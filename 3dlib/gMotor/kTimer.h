/*****************************************************************************/
/*    File:    kTimer.h
/*    Desc:    
/*    Author:    Ruslan Shestopalyuk
/*    Date:    10-04-2004
/*****************************************************************************/
#ifndef __KTIMER_H__
#define __KTIMER_H__

/*****************************************************************************/
/*    Class:    Timer
/*    Desc:    High resolution timer
/*****************************************************************************/
class Timer
{
    double                      m_TotalTime;    //  total timer time
    LARGE_INTEGER               m_TimeStart;    //  current start time 
    LARGE_INTEGER               m_Frequency;    //  timer frequency
    double                      m_Period;

    bool                        m_bPaused;

public:
    Timer()
    {
        m_bPaused = false;
        m_Period  = 0.0;
        if (QueryPerformanceFrequency( &m_Frequency ) == FALSE)
        {
            //assert( !"No performance counter available!" );
        }
        start();
    }

    Timer( float period )
    {
        m_bPaused = false;
        if (QueryPerformanceFrequency( &m_Frequency ) == FALSE)
        {
            assert( !"No performance counter available!" );
        }
        start( period );
    }

    void start( float period = 0.0f )
    {
        m_Period = period;
        reset();
        QueryPerformanceCounter( &m_TimeStart );
        m_bPaused = false;
    }

    void pause()
    {
        LARGE_INTEGER stopTime;

        QueryPerformanceCounter( &stopTime );
        m_TotalTime += (double( stopTime.QuadPart ) - double( m_TimeStart.QuadPart ))/(double)m_Frequency.QuadPart;
        m_bPaused = true;
    }

    void set( double t )
    {
        QueryPerformanceCounter( &m_TimeStart );
        m_TotalTime = t;
    }

    void reset()
    {
        m_TotalTime = 0.0;
        memset( &m_TimeStart, 0, sizeof( m_TimeStart ) );
    }
    
    float period() const { return m_Period; }

    void resume()
    {
        QueryPerformanceCounter( &m_TimeStart );
        m_bPaused = false;
    }
    
    int iteration() const 
    {
        return m_Period == 0.0f ? 0.0f : seconds( true )/m_Period; 
    }

    double seconds( bool bTotal = false ) const
    {
        if (m_bPaused) return m_TotalTime;
        LARGE_INTEGER curTime;

        QueryPerformanceCounter( &curTime );
        curTime.QuadPart -= m_TimeStart.QuadPart;
        double t = (double)curTime.QuadPart/(double)m_Frequency.QuadPart;
        t += m_TotalTime;
        if (m_Period != 0.0f && !bTotal) t = fmodf( t, m_Period );
        return t;
    }

}; // class Timer

#endif // __KTIMER_H__