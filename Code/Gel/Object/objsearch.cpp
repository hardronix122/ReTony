/*****************************************************************************
**																			**
**			              Neversoft Entertainment			                **
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		GEL (Game Engine Library)								**
**																			**
**	Module:			Objects (OBJ) 											**
**																			**
**	File name:		objsearch.cpp											**
**																			**
**	Created:		05/27/99	-	mjb										**
**																			**
**	Description:	Object search code										**
**																			**
*****************************************************************************/


/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#include <Core/Defines.h>

#include <Core/List.h>
#include <Core/Task.h>

#include <Gel/objsearch.h>
#include <Gel/mainloop.h>


/*****************************************************************************
**								  Externals									**
*****************************************************************************/

/*****************************************************************************
**								   Defines									**
*****************************************************************************/

/*****************************************************************************
**								DBG Defines									**
*****************************************************************************/

namespace Obj
{



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

CObject*			Search::FindFirstObjectOfType( Lst::Head< CObject >& head, sint type )
{
	CObject*	obj = FirstItem( head );
	
	obj_type = type;

	while ( obj )
	{
		Dbg_AssertType( obj, CObject );

		if ( obj->GetType() == obj_type )
		{
			return obj;
		}
		
		obj = NextItem();
	}
	
	return obj;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

CObject*			Search::FindNextObjectOfType( void )
{
	

	CObject*	obj = NextItem();

	while ( obj )
	{
		Dbg_AssertType( obj, CObject );
	
		if ( obj->GetType() == obj_type )
		{
			return obj;
		}
			
		obj = NextItem();
	}
	
	return obj;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

} // namespace Obj


