/*****************************************************************************
**																			**
**			              Neversoft Entertainment			                **
**																		   	**
**				   Copyright (C) 1999 - All Rights Reserved				   	**
**																			**
******************************************************************************
**																			**
**	Project:		GFX (Graphics Library)									**
**																			**
**	Module:			Graphics (GFX)		 									**
**																			**
**	File name:		gfxman.cpp												**
**																			**
**	Created:		07/26/99	-	mjb										**
**																			**
**	Description:	Graphics device manager									**
**																			**
*****************************************************************************/


/*****************************************************************************
**							  	  Includes									**
*****************************************************************************/


#include <Core/Defines.h>
#include <Core/Debug.h>

#include <Sys/Profiler.h>
#include <Sys/timer.h>

#include <Gfx/gfxman.h>
#include <Gfx/NxViewport.h>
#include <Gfx/camera.h>

#include <Gel/mainloop.h>
#include <Sys/Config/config.h>


/*****************************************************************************
**								DBG Information								**
*****************************************************************************/

Dbg_DefineProject ( GfxLib, "Graphics Library" )

/*****************************************************************************
**								  Externals									**
*****************************************************************************/

uint64 Gfx_LastVBlank = 0;

namespace Gfx
{

/*****************************************************************************
**								   Defines									**
*****************************************************************************/


/*****************************************************************************
**								Private Types								**
*****************************************************************************/


/*****************************************************************************
**								 Private Data								**
*****************************************************************************/

DefineSingletonClass( Manager, "Gfx Manager" )

/*****************************************************************************
**								 Public Data								**
*****************************************************************************/

/*****************************************************************************
**							  Private Prototypes							**
*****************************************************************************/

/*****************************************************************************
**							   Private Functions							**
*****************************************************************************/


void		Manager::s_timer_code( const Tsk::Task< Manager >& task )
{
	Manager& gfx_manager = task.GetData();

	gfx_manager.m_time += (Tmr::Time) (int) (( Tmr::FrameLength() * 60.0f ) * 
										( Tmr::vRESOLUTION / Config::FPS() ));
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/
#ifdef __PLAT_NGC__
int gDumpMem = 0;
#endif		// __PLAT_NGC__

void		Manager::s_start_render_code ( const Tsk::Hook< Manager >& hook )
{
	(void)hook;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void		Manager::s_end_render_code ( const Tsk::Hook< Manager >& hook )
{

// Note: not currently called.... just left in to show the timing stuff....
	
	Manager&	gfx_manager = hook.GetData();
    
	uint64 this_vblank;
	
	do
	{
		this_vblank = Tmr::GetVblanks();
	}
	while(( this_vblank - Gfx_LastVBlank ) < gfx_manager.m_min_vblank_wait );
	Gfx_LastVBlank = this_vblank;

	Tmr::OncePerRender();   		// update the frame counter
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void	Manager::start_engine( void )
{
	
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void	Manager::stop_engine( void )
{

}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/



Manager::Manager ( void )
{
	Mlp::Manager * mlp_man = Mlp::Manager::Instance();

	m_render_start_hook = new Tsk::Hook< Manager > ( s_start_render_code, *this );
	mlp_man->RegisterRenderStartHook ( m_render_start_hook );

	m_render_end_hook = new Tsk::Hook< Manager > ( s_end_render_code, *this );
	mlp_man->RegisterRenderEndHook ( m_render_end_hook );

	m_timer_task = new Tsk::Task< Manager > ( s_timer_code, *this );
	m_timer_task->SetMask(1<<3);
	mlp_man->AddLogicTask( *m_timer_task );
		
	m_min_vblank_wait = 0;

	m_metrics_active = false;
	m_vram_viewer_active = false;
    m_active_blend = -1;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

Manager::~Manager ( void )
{
	Dbg_AssertType ( m_render_start_hook, Tsk::Hook< Manager > );
	delete m_render_start_hook;

	Dbg_AssertType ( m_render_end_hook, Tsk::Hook< Manager > );
	delete m_render_end_hook;

	Dbg_AssertType( m_timer_task, Tsk::Task< Manager > );
	delete m_timer_task;

#ifdef __NOPT_ASSERT__
	Dbg_SetScreenAssert( false );
#endif

	stop_engine();
}


/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/


void	Manager::ToggleMetrics( void )
{
	
	
	m_metrics_active = !m_metrics_active;

#ifdef		__USE_PROFILER__
	if( m_metrics_active )
	{
		Sys::Profiler::sEnable();
	}
	else
	{
		Sys::Profiler::sDisable();
	}
#endif
}

#ifdef __NOPT_ASSERT__
/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void		Manager::AssertText ( int line, const char* text )
{
	(void)line;
	(void)text;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

void	Manager::AssertFlush( void )
{
// not needed...	

}

void Manager::ScrollActiveBlendMode() {
    m_active_blend++;

    if(m_active_blend > 17) {
        m_active_blend = -1;
    } else if(m_active_blend > 15) {
        m_active_blend = 17;
    } else if(m_active_blend > 13) {
        m_active_blend = 15;
    }

    printf("Active blend mode: %d\n", m_active_blend);
}

int Manager::GetActiveBlendMode() {
    return m_active_blend;
}

/******************************************************************/
/*                                                                */
/*                                                                */
/******************************************************************/

#endif /* __NOPT_ASSERT__ */

} // namespace Gfx
