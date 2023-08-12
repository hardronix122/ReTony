/*****************************************************************************
**																			**
**			              Neversoft Entertainment	                        **
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		Sys Library												**
**																			**
**	Module:			Memory Manager (Mem)									**
**																			**
**	Created:		03/20/00	-	mjb										**
**																			**
**	File name:		core/sys/mem/pool.h										**
**																			**
*****************************************************************************/

#ifndef	__SYS_MEM_POOL_H
#define	__SYS_MEM_POOL_H

/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/

#ifndef __CORE_DEFINES_H
#include <Core/Defines.h>
#endif
#include "alloc.h"

/*****************************************************************************
**								   Defines									**
*****************************************************************************/

namespace Mem
{



/*****************************************************************************
**							     Type Defines								**
*****************************************************************************/

class Region;

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

class  Pool : public  Allocator 			
{
	
	
	friend class Manager;

public :
		
								Pool( Region* region, size_t size, uint count, Direction dir = vBOTTOM_UP );
	virtual						~Pool( void );
	
private :
	
	virtual		void*			allocate( size_t size, bool assert_on_fail );
	virtual		void			free( BlockHeader* pHeader );
				void			dump_free_list( void );

				BlockHeader*	mp_free_list;
				uint			m_count;
				size_t			m_size;
};

/*****************************************************************************
**							 Private Declarations							**
*****************************************************************************/

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

/*****************************************************************************
**							  Public Declarations							**
*****************************************************************************/

/*****************************************************************************
**							   Public Prototypes							**
*****************************************************************************/

/*****************************************************************************
**								Inline Functions							**
*****************************************************************************/

} // namespace Mem

#endif  // __SYS_MEM_POOL_H
