/*****************************************************************************/
/*    File:    kHistory.cpp
/*    Desc:    Generic action history 
/*    Author:    Ruslan Shestopalyuk
/*    Date:    22.05.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kHistory.h"

/*****************************************************************************/
/*    CommandHistory implementation
/*****************************************************************************/
CommandHistory::CommandHistory()
{
    SetMaxCommandNum( c_DefaultMaxHistoryCmdNum );
} // CommandHistory::CommandHistory

CommandHistory::~CommandHistory()
{
    Clear();
}

bool CommandHistory::UndoCommand()
{
    if (numCmd == 0) return false;
    
    int prevCmd = curHead - 1;
    if (prevCmd < 0) prevCmd = maxCmdNum - 1;
    Command* pCmd = cmd[prevCmd];
    assert( pCmd );
    bool res = pCmd->Undo( this ); 
    //  remove undone command
    delete pCmd;
    cmd[prevCmd] = NULL;
    curHead = prevCmd;
    numCmd--;
    return res;
} // CommandHistory::UndoCommand

void CommandHistory::AddCommand( Command* pCmd )
{
    delete cmd[curHead];
    cmd[curHead] = pCmd;
    curHead = (curHead + 1) % maxCmdNum; 
    if (numCmd < maxCmdNum) numCmd++;
} // CommandHistory::AddCommand

void CommandHistory::Clear()
{
    for (int i = 0; i < cmd.size(); i++)
    {
        delete cmd[i];
        cmd[i] = 0;
    }
    curHead = 0;
    numCmd  = 0;
    
} // CommandHistory::Clear

void CommandHistory::SetMaxCommandNum( int _num )
{
    maxCmdNum = _num;
    for (int i = 0; i < maxCmdNum; i++)
    {
        cmd.push_back( NULL );
    } 
    curHead = 0;
    numCmd  = 0;
}  // CommandHistory::SetMaxCommandNum



