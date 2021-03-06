//=======================================================================================================================|
// Created 2013.11.16 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "OpenGLWindow.h"

namespace glz
{
	OpenGLWindow::OpenGLWindow()
	{
		mProgram = -1;
		mWidth = gDefaultWindowWidth;
		mHeight = gDefaultWindowHeight;
		mTitle = gWindowTitle;
		mWindow = nullptr;
		mAspectScale = Vec2f(1.0f, 1.0f);
		mZoom = Vec2f(0.2f, 0.2f);
		mShouldTerminate = false;
	}

	
	int OpenGLWindow::init(int width, int height, String title)
	{
		mWidth = width;
		mHeight = height;
		mTitle = title;

		if (!glfwInit())
			return -1;
		
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

		mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!mWindow)
		{
			terminate();
			glfwTerminate();
			return -1;
		}

		glfwMakeContextCurrent(mWindow);
		gl3wInit();

		//if (gl3wIsSupported(4, 4) == false)
		//	printf("Bad OpenGL version.\n");
		GLFWInput::instance();
		glfwSetKeyCallback(mWindow, reinterpret_cast<GLFWkeyfun>(GLFWInput::keyboardCallback));
		glfwSetWindowSizeCallback(mWindow, GLFWInput::windowResizeCallback);
		GLFWInput::instance().addWindow(this);

		onStartup();

		//get the aspect scale adjusted
		_onResize(mWidth, mHeight);
		sf::Clock clock;

		while (!glfwWindowShouldClose(mWindow)  &&  !mShouldTerminate)
		{
			onUpdate(double(clock.getElapsedTime().asSeconds()));

			glfwSwapBuffers(mWindow);
			glfwPollEvents();
		}

		onTerminate();
		glfwTerminate();

		return 0;
	}


	void OpenGLWindow::setZoom(Vec2f zoom)
	{
		mZoom = zoom;

		if (mZoom.x < 0.0) mZoom.x = 0.0;
		if (mZoom.y < 0.0) mZoom.y = 0.0;

		_onResize(mWidth, mHeight);
	}


	void OpenGLWindow::addZoom(Vec2f zoom)
	{
		mZoom += zoom;

		if (mZoom.x < 0.0) mZoom.x = 0.0;
		if (mZoom.y < 0.0) mZoom.y = 0.0;

		_onResize(mWidth, mHeight);
	}

	
	void OpenGLWindow::_onResize(int width, int height)
	{
		mAspectScale = Vec2f((500.0f/width)*mZoom.x, (500.0f/height)*mZoom.y);
		mWidth = width;
		mHeight = height;

		onResize(width, height);
	}
};