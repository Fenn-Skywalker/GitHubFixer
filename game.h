#pragma once
#include "surface.h"
#include "template.h"
#include "windows.h"
#include "player.h"
#include "sprites.h"
#include "layers.h"
#include "entities.h"
#include <LDtkLoader/Project.hpp>
#include <iostream>
#include <vector>

namespace Tmpl8 {

	class Surface;
	class Game
	{
	public:

		void setHitboxes();
		void SetTarget(Surface* surface) { screen = surface; }
		void Init();
		void Shutdown();
		void Tick(float deltaTime);
		void MouseUp(int button) { /* implement if you want to detect mouse button presses */ }
		void MouseDown(int button) { /* implement if you want to detect mouse button presses */ }
		void MouseMove(int x, int y) { /* implement if you want to detect mouse movement */ }
		void KeyUp(int key) { /* implement if you want to handle keys */ }
		void KeyDown(int key) { /* implement if you want to handle keys */ }
		void loadLevel(int lvlNum);
		void getNextLevel();

	private:

		Surface* screen;
		Surface* tiles;

		Player player; // Player object
		vec2 playerPos;
		vec2 startPos;

		Layers layer;
		Layers bgLayer;

		HBox playerSize;
		HBox startPosition;

		Sprites playersprite;
		Sprites keysprite;
		Sprites doorsprite;

		Key key;
		Door door;
		Text text;

		std::shared_ptr<SpriteSheet> charactersheet;
		std::shared_ptr<SpriteSheet> keysheet;
		std::shared_ptr<SpriteSheet> textsheet;

		int currentLevel;

		std::vector<HBox> hitboxes;
		std::vector<HBox> keys;
		std::vector<HBox> doorBox;
		std::vector<HBox> textBox; // for the text box
	};

}; // namespace Tmpl8