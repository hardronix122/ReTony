/*****************************************************************************
**																			**
**					   	  Neversoft Entertainment							**
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		PS2														**
**																			**
**	Module:			Scripting												**
**																			**
**	File name:		ftables.h												**
**																			**
**      Created:                09/15/2000      -        ksh                                                                             **
**																			**
*****************************************************************************/

#ifndef	__SCRIPTING_FTABLES_H
#define	__SCRIPTING_FTABLES_H

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#ifndef __CORE_DEFINES_H
#include <Core/Defines.h>
#endif

#ifndef	__SCRIPTING_INIT_H
#include <Gel/Scripting/init.h>
#endif

/*****************************************************************************
**								   Defines									**
*****************************************************************************/
namespace Script
{

/*****************************************************************************
**							Class Definitions								**
*****************************************************************************/

/*****************************************************************************
**							 Private Declarations							**
*****************************************************************************/

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

/*****************************************************************************
**							  Public Declarations							**
*****************************************************************************/

extern SCFunction CFunctionLookupTable[];

extern const char *ppMemberFunctionNames[];

/*****************************************************************************
**							   Public Prototypes							**
*****************************************************************************/

int GetCFunctionLookupTableSize();
int GetNumMemberFunctions();

/*****************************************************************************
**								Inline Functions							**
*****************************************************************************/

} // namespace FTables

#endif	// __SCRIPTING_FTABLES_H

