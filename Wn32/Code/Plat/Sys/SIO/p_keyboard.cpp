/*****************************************************************************
**																			**
**			              Neversoft Entertainment			                **
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		SYS Library												**
**																			**
**	Module:			SYS (SYS_) 												**
**																			**
**	File name:		keyboard.cpp											**
**																			**
**	Created:		03/08/2001	-	gj										**
**																			**
**	Description:	USB Keyboard interface									**
**																			**
*****************************************************************************/


/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#include <Core/Defines.h>
#include <Sys/SIO/keyboard.h>

/*****************************************************************************
**								DBG Information								**
*****************************************************************************/

namespace SIO
{

/*****************************************************************************
**								  Externals									**
*****************************************************************************/


/*****************************************************************************
**								Private Types								**
*****************************************************************************/


/*****************************************************************************
**								 Private Data								**
*****************************************************************************/


/*****************************************************************************
**								 Public Data								**
*****************************************************************************/


/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/


/*****************************************************************************
**							   Private Functions							**
*****************************************************************************/


/*****************************************************************************
**							   Public Functions								**
*****************************************************************************/

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

int KeyboardInit(void)
{
	return 0;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

int KeyboardDeinit(void)
{
	return 0;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

int KeyboardRead( char* makes )
{
	(void)makes;
	return 0;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void KeyboardClear( void )
{
}


/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

} // namespace SIO
