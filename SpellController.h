#ifndef __SPELLCONTROLLER_H
#define __SPELLCONTROLLER_H

#include "Particle_Master.h"

class Character;

struct SpellSet {
	SpellSet() : owner(nullptr), target(nullptr), currentSpell(0) {}
	Character *owner;
	Character *target;
	std::vector<Particle_Property> spell;
	int currentSpell;
	std::vector<int> timeRemain;
	std::list<Particles_V2>::iterator particleIter;
};

class SpellController {
public:
	SpellController();
	~SpellController();

	void SetParticleMaster(Particle_Master *particleMaster);
	void RegisterSpell(const SpellSet &spellSet);
	void Update();

	std::list<SpellSet> spellSets;
	Particle_Master *particleMaster;

private:
	void ActivateSpell(std::list<SpellSet>::iterator setIter, const int &spellIndex);
};
#endif