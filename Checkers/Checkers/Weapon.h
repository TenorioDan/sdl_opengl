#pragma once
#include <vector>
#include "Projectile.h"

class Weapon 
{
public:
	Weapon();
	~Weapon();

	bool loadMedia();
	void update(int time);
	void render();

	void fireWeapon(GLfloat positionX, GLfloat positionY, GameObject::Direction direction, GLfloat aimDirectionX, GLfloat aimDirectionY);
private:
	std::vector<Projectile*> projectiles;
	SpriteSheet projectileSpriteSheet;
};