#ifndef __GLOBALFUNCTIONS_H
#define __GLOBALFUNCTIONS_H

#pragma once

#include <Windows.h>
#include <time.h>
#include <iostream>

#include <glm/glm.hpp>
#include <assimp/scene.h>


class GlobalFunctions {
public:
	GlobalFunctions() : start(0), end(0), timeStamp(0) {}
	~GlobalFunctions() {};

	void Start();
	void End(char *msg);
	int End();
	int CalcTimeElapsed();
	void SetAnimTimestamp();
	int GetAnimTimestamp();
	void PrintMatrix(glm::mat4 matrix);
	//void PrintMatrix(aiMatrix4x4 matrix);
	glm::mat4 AiToGlm(const aiMatrix4x4 &matrix);
	bool FpsCounter();
	float CalcLength(glm::vec3 vector);

private:
	int start, end;
	int animTimeStamp;
	int fpsCount_start;
	bool fpsCount_flag;
	int timeStamp;
};

#endif