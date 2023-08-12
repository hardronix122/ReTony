/*****************************************************************************
**																			**
**			              Neversoft Entertainment							**
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Core Library											**
**																			**
**	Module:			Task Manager (TSK)										**
**																			**
**	File name:		task.cpp												**
**																			**
**	Created by:		05/27/99	-	mjb										**
**																			**
**	Description:	Task Support											**
**																			**
*****************************************************************************/

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#include <Core/Defines.h>
#include <Core/Task.h>

/*****************************************************************************
**							  DBG Information								**
*****************************************************************************/



namespace Tsk
{



/*****************************************************************************
**								   Externals								**
*****************************************************************************/


/*****************************************************************************
**								   Defines									**
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

BaseTask::BaseTask( Node::Priority priority )
: tlist(nullptr), stamp(0)
{
	

	m_mask = 0;							// default to "No type of thing", so it will always run
	node = new Node( this, priority );
	Dbg_AssertType( node, Node );
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

BaseTask::~BaseTask ()
{
	
	
	if ( tlist )
	{   
		Remove();
	}

	Dbg_AssertType( node, Node );
	delete node;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void	BaseTask::Remove( void )
{
	
	
	if ( tlist )
	{
		tlist->Forbid();

		tlist->SignalListChange();
		node->Remove();

		tlist->Permit();
		tlist = nullptr;
	}
	else
	{
		Dbg_Warning( "Task is not in a List" );
	}

}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
 
bool		BaseTask::InList( void ) const
{
	

	return	node->InList();
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void		BaseTask::SetPriority( Node::Priority pri ) const
{
	

	node->SetPri( pri );
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

} // namespace Tsk
