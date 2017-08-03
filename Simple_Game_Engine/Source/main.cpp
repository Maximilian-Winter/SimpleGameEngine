/*
Simple Deferred 3D Engine
Author: Maximilian Winter
Date: 01.12.2014

To Do:

- Optimize Physically Based Shading** 
- Complete Forward Rendering Path**
- Change Mathlibary to XNAMATH***
- Add Frustum Culling**
- Redesign and complete Camerclass*

*/

// Includes
#include "System.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	System System;
	bool result;

	// Initialize and run the system object.
	result = System.Initialize();
	if(result)
	{
	//MessageBeep();
		System.Run();
	}

	// Shutdown the system object.
	System.Shutdown();

	return 0;
}