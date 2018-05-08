#include "SpellController.h"
#include "GlobalFunctions.h"
#include "Character.h"

using namespace std;
using namespace glm;

SpellController::SpellController() {

}

SpellController::~SpellController() {

}

void SpellController::SetParticleMaster(Particle_Master *particleMaster) {
	this->particleMaster = particleMaster;
}

void SpellController::RegisterSpell(const SpellSet &spellSet) {
	spellSets.push_back(spellSet);
	ActivateSpell(--spellSets.end(), 0);
}

void SpellController::ActivateSpell(list<SpellSet>::iterator setIter, const int &spellIndex) {
	setIter->spell[0].origin = vec3(setIter->owner->iter->HandMatrix * vec4(0, 0, 0, 1)) - vec3(0, 2, 0);
	setIter->particleIter = particleMaster->GenParticle(setIter->spell[spellIndex]);
}

void SpellController::Update() {
	auto setIter = spellSets.begin();
	for (; setIter != spellSets.end(); ++setIter) {
		// Update spell origin
		setIter->particleIter->origin = vec3(setIter->owner->iter->HandMatrix * vec4(0, 0, 0, 1)) - vec3(0, 2, 0);

		setIter->timeRemain[setIter->currentSpell] -= GlobalFunctions::timeElapsed;
		// Proceed to next spell
		if (setIter->timeRemain[setIter->currentSpell] < 0) {
			if (setIter->currentSpell == 0) {
				if (setIter->owner->animState == STATE_CASTING) {
					setIter->owner->SetState(STATE_IDLE);
				}
			}
			setIter->currentSpell++;
			// End of spell sequence
			if (setIter->currentSpell == setIter->spell.size()) {
				spellSets.erase(setIter);
				--setIter;
			}
			else {
				if (setIter->target != nullptr) {
					setIter->spell[setIter->currentSpell].initDir = normalize(setIter->target->collider.center - setIter->particleIter->origin);
				}
				
				/*if (setIter->owner->iter->tag == TAG_ENEMY) {
					setIter->spell[setIter->currentSpell].initDir = -setIter->owner->GetDir();
				}
				else {
					setIter->spell[setIter->currentSpell].initDir = setIter->owner->GetDir();
				}*/
				setIter->spell[setIter->currentSpell].origin = setIter->particleIter->origin;
				particleMaster->GenParticle(setIter->spell[setIter->currentSpell]);
			}
		}
		// Cancel channeling
		else {
			if (setIter->currentSpell == 0) {
				if (setIter->owner->animState != STATE_CASTING) {
					particleMaster->particles.erase(setIter->particleIter);
					spellSets.erase(setIter);
					--setIter;
				}
			}
		}
	}
}