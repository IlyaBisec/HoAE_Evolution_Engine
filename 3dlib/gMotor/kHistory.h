/*****************************************************************************/
/*    File:    kHistory.h
/*    Desc:    Generic action history 
/*    Author:    Ruslan Shestopalyuk
/*    Date:    22.05.2003
/*****************************************************************************/
#ifndef __KHISTORY_H__
#define __KHISTORY_H__

const int c_DefaultMaxHistoryCmdNum = 64;

class Command;
/*****************************************************************************/
/*    Class:    CommandHistory
/*    Desc:    Represents sequence of the commands
/*****************************************************************************/
class CommandHistory
{
    std::vector<Command*>        cmd;
    int                        maxCmdNum;
    int                        curHead;
    int                        numCmd;
public:
            CommandHistory();
    virtual ~CommandHistory();

    bool    UndoCommand();
    void    AddCommand( Command* pCmd );
    void    Clear();
    void    SetMaxCommandNum( int _num );

}; // class CommandHistory

/*****************************************************************************/
/*    Class:    Command
/*    Desc:    Single action in the history
/*****************************************************************************/
class Command
{
public:
    virtual bool Undo( CommandHistory* pHistory ) = 0;
}; // class Command

#endif // __KHISTORY_H__