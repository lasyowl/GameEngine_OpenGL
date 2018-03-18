#include "Model_Animation.h"

Model_Animation::Model_Animation() :recentRatio(0) {
	
}

Model_Animation::~Model_Animation() {

}

void Model_Animation::operator = (const aiAnimation *anim) {
	name = anim->mName.data;
	//name = name.substr(sizeof("AnimStack:"), name.size() - 11);
	numChannels = anim->mNumChannels;
	channels.resize(numChannels);
	duration = anim->mDuration;
	tickPerSec = anim->mTicksPerSecond;

	for (int i = 0; i < numChannels; i++) {
		channels[i] = anim->mChannels[i];
	}
}