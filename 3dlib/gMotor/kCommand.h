/*****************************************************************************/
/*    File:    kCommand.h
/*    Author:    Ruslan Shestopalyuk
/*    Date:    22.05.2003
/*****************************************************************************/
#ifndef __KCOMMAND_H__
#define __KCOMMAND_H__

const int c_DefaultMaxHistoryCmdNum = 64;

/*****************************************************************************/
/*    Enum:    CtrlFlags
/*****************************************************************************/
enum CtrlFlags
{
    cfControl    = 1 << 1,
    cfAlt        = 1 << 2,
    cfShift        = 1 << 3,
    cfTab        = 1 << 4,
    cfEsc        = 1 << 5,
    cfLeft        = 1 << 6,
    cfRight        = 1 << 7,
    cfUp        = 1 << 8,
    cfDown        = 1 << 9,
    cfBackSpace    = 1 << 10,
    cfDel        = 1 << 11,
    cfIns        = 1 << 12,
    cfHome        = 1 << 13,
    cfPgUp        = 1 << 14,
    cfPgDown    = 1 << 15
}; // enum CtrlFlags

/*****************************************************************************/
/*    Class:    Hotkey
/*****************************************************************************/
class Hotkey
{
    DWORD                    m_Char;
    DWORD                    m_CtlKeys;

public:

    Hotkey() : m_Char(0), m_CtlKeys(0) {}
    Hotkey( char ch, DWORD ctlKeys = 0 ) : m_Char(ch), m_CtlKeys(ctlKeys) {}
    Hotkey( int    vkCode, DWORD ctlKeys = 0 ) : m_Char(vkCode), m_CtlKeys(ctlKeys) {}

    bool    operator == ( const Hotkey& hk ) 
    { return (m_Char == hk.m_Char) && (m_CtlKeys == hk.m_CtlKeys); }

    static Hotkey            None;
}; // class Hotkey

/*****************************************************************************/
/*    Class:    Command
/*    Desc:    Single action type
/*****************************************************************************/
class Command
{
    c2::string            m_Name;
    c2::string            m_Description;
    Hotkey                m_Hotkey;
    bool                m_bUndoable;

public:

                        Command    () : m_bUndoable(false) {}
    virtual bool        Undo    (){ return false; }
    virtual bool        Execute    (){ return false; }
}; // class Command

/*****************************************************************************/
/*    Class:    CommandInstance
/*    Desc:    Concrete instance of the user action
/*****************************************************************************/
//class Command
//{
//    c2::string            m_Name;
//    c2::string            m_Description;
//    Hotkey                m_Hotkey;
//    bool                m_bUndoable;
//
//public:
//
//    Command    () : m_bUndoable(false) {}
//    virtual bool        Undo    (){ return false; }
//    virtual bool        Execute    (){ return false; }
//}; // class Command

/*****************************************************************************/
/*    Class:    CommandHistory
/*    Desc:    Represents history of the exucuted commands
/*****************************************************************************/
template <int MaxCommands = c_DefaultMaxHistoryCmdNum>
class CommandHistory
{
    c2::circular_queue<Command*, MaxCommands>    m_Command;

public:
                    CommandHistory    ();
    virtual            ~CommandHistory    ();

    bool UndoLastCommand() 
    { 
        if (m_Command.numEl() == 0) return false;
        return m_Command.pop()->Undo();
    }
    
    void AddCommand( Command* pCmd )
    {
        m_Command.push();
    }

    void Clear()
    {
        m_Command.clear();
    }

}; // class CommandHistory

#endif // __KCOMMAND_H__