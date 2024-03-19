/*****************************************************************************/
/*	File:	uiSequencer.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	11-23-2003
/*****************************************************************************/
#ifndef __UISEQUENCER_H__
#define __UISEQUENCER_H__

namespace sg{

/*****************************************************************************/
/*	Class:	Sequencer
/*	Desc:	Dialog for mixing timed tracks into complex animation
/*****************************************************************************/
class Sequencer : public Dialog
{
public:
	Sequencer(){}

	NODE(Sequencer,Dialog,SEQRs);
}; // class Sequencer

}; // namespace sg

#endif // __UISEQUENCER_H__