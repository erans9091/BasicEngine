#include "game.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>
#define THR_HOLD 75

static GLFWimage *loadImgToOneDArr(const string &path)
{
	GLFWimage *img = new GLFWimage;
	int numComponents;
	img->pixels = stbi_load((path).c_str(), &(img->width), &(img->height), &numComponents, 1);
	if (img->pixels == NULL)
	{
		std::cout << "can't open image" << std::endl;
		stbi_image_free(img->pixels);
		delete img;
		return nullptr;
	}
	return img;
}

byte gradOnPix(unsigned char *img, int pixX, int pixY, int w)
{
	if (pixX == 0 || pixY == 0 || pixX == MAXWID || pixY == MAXWID)
	{
		return 0;
	}
	int gx = /*img->pixels[pixY * img->width + pixX]*/ -img[pixY * w + pixX - 1] + img[pixY * w + pixX + 1];
	int gy = /*img->pixels[pixY * img->width + pixX]*/ -img[(pixY - 1) * w + pixX] +
			 img[(pixY + 1) * w + pixX];
	return static_cast<byte>(std::sqrt(gx * gx + gy * gy));
	/*[0 -1 0]
	  [-1 0 1]
	  [0 1 0]*/
}

byte *edgeDetection(unsigned char *img, int width, int height)
{
	byte *newImg = new unsigned char[width * height];
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			newImg[y * width + x] = gradOnPix(img, x, y, width) > THR_HOLD ? 255 : 0;
			// if(y==70) std::cout << x << "\n";
		} // std::cout << y<<", ";
	}
	return newImg;
}

void half_toning_fill(unsigned char *new_img, int new_x, int new_y, unsigned char mask[], int new_width)
{
	new_img[(new_y * new_width) + new_x] = mask[0];			// mapped pixel
	new_img[(new_y * new_width) + new_x + 1] = mask[1];		// 1 right
	new_img[(new_y + 1) * new_width + new_x + 1] = mask[2]; // 1 bottom, 1 right
	new_img[(new_y + 1) * new_width + new_x] = mask[3];		// 1 below
}

unsigned char *halftoning_algo(unsigned char *img_data, int width, int height)
{
	int num_of_intensities = 256;
	double max_intensity = num_of_intensities - 1;
	int new_height = 2 * height;
	int new_width = 2 * width;
	int slot;																 // index of mask filter to be chosen
	unsigned char *new_img_data = new unsigned char[new_height * new_width]; // dynamic memory to assign an initial 0 value
	unsigned char masks[5][4] = {{0, 0, 0, 0},								 // 5 different masks - 2*2 + 1, indexed 0 to 4, of increasing intensity (0 mask all black .. 4 all white) mask will be picked based on source pixel calculation
								 {0, 0, 0, 255},
								 {255, 0, 255, 0},
								 {255, 255, 0, 255},
								 {255, 255, 255, 255}};

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			slot = (int)(((double)img_data[y * width + x] / max_intensity) * 5);
			if (slot == 5)
			{
				slot = 4;
			};
			half_toning_fill(new_img_data, 2 * x, 2 * y, masks[slot], new_width);
		}
	}
	return new_img_data;
}

unsigned char *floyd_steinberg_algo(unsigned char *img_data, int width, int height)
{
	
	unsigned char *new_img_data = new unsigned char[height * width];

	// clear new image
	for (int i = 0; i < height * width; i++)
	{
		new_img_data[i] = 0;
	}

	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int current = img_data[y * width + x];
			int accumulatedError = new_img_data[y * width + x];

			int newPixel = int(float(current + accumulatedError) / 256 * 16);
			if(newPixel > 15) newPixel = 15;

			new_img_data[y * width + x] = newPixel * 16;

			int error = abs(current - newPixel * 16);

			if (x + 1 < width)
			{
				new_img_data[y * width + x + 1] += int(error * 7 / 16);
			}
			if (x - 1 >= 0 && y + 1 < height)
			{
				new_img_data[(y + 1) * width + x - 1] += int(error * 3 / 16);
			}
			if (y + 1 < height)
			{
				new_img_data[(y + 1) * width + x] += int(error * 5 / 16);
			}
			if (x + 1 < width && y + 1 < height)
			{
				new_img_data[(y + 1) * width + x + 1] += int(error / 16);
			}
		}
	}
	return new_img_data;
}

static void printMat(const glm::mat4 mat)
{
	std::cout << " matrix:" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			std::cout << mat[j][i] << " ";
		std::cout << std::endl;
	}
}

Game::Game() : Scene()
{
}

Game::Game(float angle, float relationWH, float near1, float far1) : Scene(angle, relationWH, near1, far1)
{
}

void Game::Init()
{

	AddShader("../res/shaders/pickingShader");
	AddShader("../res/shaders/basicShader");

	// AddTexture("../res/textures/grass.bmp",false);

	AddShape(Plane, -1, TRIANGLES);

	pickedShape = 0;

	// SetShapeTex(0,0);
	MoveCamera(0, zTranslate, 10);
	pickedShape = -1;

	// ReadPixel(); //uncomment when you are reading from the z-buffer
}

void Game::Update(const glm::mat4 &MVP, const glm::mat4 &Model, const int shaderIndx)
{
	Shader *s = shaders[shaderIndx];
	int r = ((pickedShape + 1) & 0x000000FF) >> 0;
	int g = ((pickedShape + 1) & 0x0000FF00) >> 8;
	int b = ((pickedShape + 1) & 0x00FF0000) >> 16;
	s->Bind();
	s->SetUniformMat4f("MVP", MVP);
	s->SetUniformMat4f("Normal", Model);
	s->SetUniform4f("lightDirection", 0.0f, 0.0f, -1.0f, 0.0f);
	if (shaderIndx == 0)
		s->SetUniform4f("lightColor", r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
	else
		s->SetUniform4f("lightColor", 0.7f, 0.8f, 0.1f, 1.0f);
	s->Unbind();
}

void Game::WhenRotate()
{
}

void Game::WhenTranslate()
{
}

void Game::Motion()
{
	if (isActive)
	{
	}
}

Game::~Game(void)
{
}

void write_img_to_file(const char *filename, unsigned char *img_data, int length_bytes, int width, bool is_binary)
{
	std::fstream img5_ostream;
	img5_ostream.open(filename, std::fstream::out | std::fstream::trunc); // stream open for writing, override any data on the file if already exists
	if (img5_ostream.is_open())
	{
		for (int i = 0; i < length_bytes; i++)
		{
			int intensity = img_data[i];
			if (intensity && is_binary)
			{
				intensity = 1;
			};
			img5_ostream << intensity << ",";
			if (i % width == 0)
			{
				img5_ostream << "\n";
			}
		}
	}
	else
	{
		std::cout << "Couldn't open the text file with name: " << filename << std::endl;
	}

	img5_ostream.close();
}