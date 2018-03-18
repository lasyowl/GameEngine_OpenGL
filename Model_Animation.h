#ifndef __MODEL_ANIMATION_H
#define __MODEL_ANIMATION_H

#include <vector>
#include <string>

#include "Model_NodeAnim.h"

class Model_Animation {
public:
	Model_Animation();
	~Model_Animation();

	std::string name;
	int numChannels;
	std::vector<Model_NodeAnim> channels;
	float duration;
	float tickPerSec;
	float recentRatio;

	void operator = (const aiAnimation *anim);

private:

};
#endif