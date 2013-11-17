//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013 Daniel L. Watkins
//=======================================================================================================================|

#include "OpenGLApplication.h"

namespace glz
{
	OpenGLApplication::OpenGLApplication()
	{
		mWidth = gDefaultWindowWidth;
		mHeight = gDefaultWindowHeight;
		mTitle = gDefaultWindowTitle;
	}


	Int OpenGLApplication::init(Int width, Int height, String title)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;

		GLFWwindow* window;

		if (!glfwInit())
			return -1;

		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!window)
		{
			terminate();
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(window);
		gl3wInit();

		//if (gl3wIsSupported(4, 4) == false)
		//	printf("Bad OpenGL version.\n");

		startup();

		while (!glfwWindowShouldClose(window))
		{
			render(0.0); //TODO get a time value for this call

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		terminate();
		glfwTerminate();

		return 0;
	}
};