//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013 Daniel L. Watkins
//=======================================================================================================================|

#include "glZombies.h"

namespace glz
{
	glZombies::glZombies()
	{
		mProgram = -1;
	}


	void glZombies::startup()
	{
		loadShaders();
	}


	void glZombies::render(double currentTime)
	{
		static const GLfloat clearColor[] = { 1.0f, 1.0f, 0.9f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, clearColor);
	}


	void glZombies::terminate()
	{
	}



	void glZombies::loadShaders()
	{
	}
};