#include "Weapon.h"

Weapon::Weapon()
{
	damage = 20.f;
}

Weapon::~Weapon()
{
	for (auto p : projectiles)
	{
		delete p;
	}
}

bool Weapon::loadMedia()
{
	if (!projectileSpriteSheet.loadTextureFromFileWithColorKey("blaster-1.png", 255, 0, 255))
	{
		printf("Unable to load blaster sprite sheet\n");
		return false;
	}

	projectileSpriteSheet.createAnimations(20.f, 20.f, 0.f, 0.f, 0.f, 1);

	if (!projectileSpriteSheet.generateDataBuffer())
	{
		printf("Unable to clip blaster sprite sheet");
		return false;
	}

	return true;
}

// Spawn a new projectile into the world and set it's position according to the object that owns it
void Weapon::fireWeapon(GLfloat positionX, GLfloat positionY, GameObject::Direction direction, GLfloat aimDirectionX, GLfloat aimDirectionY,
						std::vector<Enemy*>* enemies)
{
	Projectile* p = new Projectile(positionX, positionY, direction, aimDirectionX, aimDirectionY, damage);
	projectiles.push_back(p);
}

// Weapon update method: Loop through all projectiles that have been fired and update them. If
// during the projectile's update call it collides with a GameObject, delete it from memory and
// remove it from the vector
void Weapon::update(int time)
{
	for (auto it = projectiles.begin(); it != projectiles.end();)
	{
		Projectile* p = *it;
		p->update(time);

		if (p->ShouldBeDeleted())
		{
			delete p;
			it = projectiles.erase(it);
		}
		else
		{
			++it;
		}

	}
}

// Draw all projectiles that have been fired
void Weapon::render()
{
	for (auto projectile : projectiles)
	{
		projectile->render();
	}
}