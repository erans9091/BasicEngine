#pragma once
#include "scene.h"

class Game : public Scene
{
public:
	
	Game();
	Game(float angle,float relationWH,float near, float far);
	void Init();
	void Update(const glm::mat4 &MVP,const glm::mat4 &Model,const int  shaderIndx); //VIRTUAL FUNCTION (PURE!!) of the Scene class
	void ControlPointUpdate(); 
	void WhenRotate();  //VIRTUAL FUNCTION (NOT PURE) of Scene class
	void WhenTranslate(); //VIRTUAL FUNCTION (NOT PURE) of Scene class
	void Motion(); //VIRTUAL FUNCTION (NOT PURE) of Scene class
	~Game(void);
};

