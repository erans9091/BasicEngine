#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include <gl/GL.h>
#include "RayTracer.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>


int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 856;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	const char* sceneFileName = "scene1.txt";

	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);
	
	
	
	glViewport(0, 0, 800, 856);
	RayTracer raytracer = RayTracer(std::string(sceneFileName));
	raytracer.RayTrace();
	unsigned char* text_data = raytracer.getTextureData();
	
	/*
	for (int y = 0; y < 256 < y++) {
		for (int x = 0; x < 256 < x++) {
		//	std::cout << "r = " << 
		}
	}
	*/

	int writeStatus = stbi_write_png("raytracedimage.png", 800, 856, 4, raytracer.getTextureData(), 800 * 4);
	std::cout << "wrtie status=" << writeStatus << std::endl;
	//scn->AddTexture(256,256, raytracer.getTextureData());
		scn->AddTexture(std::string("raytracedimage.png"), 1);
	//glViewport(0, 256, 256, 256);
	//glViewport(256, 0, 256, 256);
	//glViewport(256, 256, 256, 256);
	
	
	

	scn->Init();

	display.SetScene(scn);

	while(!display.CloseWindow())
	{
		scn->Draw(1,0,scn->BACK,true,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();	
			
	}
	delete scn;
	return 0;
}
