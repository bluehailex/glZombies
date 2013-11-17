//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013 Daniel L. Watkins
//=======================================================================================================================|

#ifndef _GL_ZOMBIES_H
#define _GL_ZOMBIES_H

#include "Main.h"
#include "OpenGLApplication.h"

namespace glz
{
	//============================================================================|
	// The actual glZombies main window class.
	//============================================================================|
	class glZombies : public OpenGLApplication
	{
	private:
		Uint mProgram;


	public:
		glZombies();


	private:
		//==================================================================|
		// -Called after the window has been fully initialized
		//==================================================================|
		void startup();

		//==================================================================|
		// -Called at the start of the window's update loop
		// -GLFW swaps buffers after this (double buffering)
		// -GLFW polls input events after this
		//==================================================================|
		void update(Double currentTime);

		//==================================================================|
		// -Called when the window closes or encounters an error
		//==================================================================|
		void terminate();

		//==================================================================|
		// -Loads, compiles, and links certain shaders into @mProgram
		//==================================================================|
		void loadShaders();
	};
};

#endif
