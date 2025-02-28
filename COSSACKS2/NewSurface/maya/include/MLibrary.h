
#ifndef _MLibrary
#define _MLibrary
//
//-
// ==========================================================================
// Copyright  (C)  Alias Systems,  a division  of  Silicon Graphics  Limited.
// All rights  reserved.  These coded  instructions,  statements and computer
// programs  contain  unpublished information  proprietary to  Alias Systems,
// a  division  of  Silicon  Graphics  Limited,  which  is  protected by  the
// Canadian  and  US federal copyright law and  may not be disclosed to third
// parties or  copied  or  duplicated,  in  whole  or in part,  without prior
// written  consent  of Alias Systems, a division of Silicon Graphics Limited
// ==========================================================================
//+
//
// CLASS:    MLibrary
//
// *****************************************************************************
//
// CLASS DESCRIPTION (MLibrary)
//
//	When creating a "library mode" Maya application, this class must be
//	used in the program's main routine to initialize Maya, and later
//	to cleanUp allocated Maya state.  A typical usage would be:
//     main(int argc, char **argv)
//     {
//         MStatus status;
//         status = MLibrary::initialize (argv[0], true);
//         if ( !status ) {
//             status.perror("MLibrary::initialize");
//             return (1);
//         }
// 
//         // Maya API code goes here
// 
//         MLibrary::cleanup();
//         return (0);
//     }
//
//  If the call to cleanup is omitted, you will get errors when the program
//  exits, as static destructors in the Maya libraries are run.
//
// *****************************************************************************

#if defined __cplusplus

// *****************************************************************************

// INCLUDED HEADER FILES



#include "MStatus.h"
#include "MTypes.h"

// *****************************************************************************

// DECLARATIONS

// *****************************************************************************

// CLASS DECLARATION (MLibrary)

/// Set up Maya to run in library mode.
/**
 Initialize and cleanup routines for Maya running in library mode.
*/
#ifdef _WIN32
#pragma warning(disable: 4522)
#endif // _WIN32

class OPENMAYA_EXPORT MLibrary  
{
public:
	///
						MLibrary ();
	///
	virtual				~MLibrary ();
	///
	static MStatus		initialize (char* applicationName,
									bool viewLicense = false);
	///
	static MStatus		initialize (bool wantScriptOutput,
									char* applicationName,
									bool viewLicense = false);
	///
	static void			cleanup( int exitStatus = 0 );

protected:
// No protected members

private:
// No private members

};

#ifdef _WIN32
#pragma warning(default: 4522)
#endif // _WIN32

// *****************************************************************************
#endif /* __cplusplus */
#endif /* _MLibrary */
