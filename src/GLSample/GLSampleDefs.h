#pragma once

// Operation mode
enum ParaViewOperation
{ 
	Idle,
	Rotate,
	Zoom,
	Pan,
	Pick
};

//*****************************************************************************
// MODULE:	Default
// DESC:	Defines the default values for various settings

#define ROTATE_SENSITIVITY	0.4
#define PAN_SENSITIVITY		0.002
#define ZOOM_SENSITIVITY	0.01

#define MIN_SCALE_FACTOR	0.0 

#define	MAX_SPHERE_SIZE		1000

#define THREEDIM			3

//*****************************************************************************
// MODULE:	Macros
// DESC:	Defines the utility macros

#ifdef NOCHECK_GL
	#define VERIFY_GL( call ) { call; }	
#else 
	#define VERIFY_GL( call ) { call; ASSERT( glGetError() == GL_NO_ERROR ); }	
#endif
