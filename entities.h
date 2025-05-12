#pragma once
#include "sprites.h"
#include "player.h"
#include <LDtkLoader/Project.hpp>

class Hitbox
{
public:
	Hitbox() = default; // Default constructor
	Hitbox(const HBox& hitbox); // Constructor with parameters
	~Hitbox() = default; // Default destructor

	void Draw(Tmpl8::Surface* surface, const Tmpl8::vec2& pos); // Draw the hitbox

private:
	HBox hitbox; // The hitbox
};

class Key
{
public:
	Key() = default; // Default constructor
	~Key() = default; // Default destructor
	void DrawKeys(Tmpl8::Surface* surface, std::vector <HBox> collisionObject, Tmpl8::vec2 playerPosition, HBox playerSize, Sprites sprite); // Add the key
	void resetCheck();// Reset the key
	void setKeys(int key) { keys = key; } // Set the keys
	int getKeys() { return keys; } // Get the keys
	void setTotalKeys(int tkey) { totalkeys = tkey; } // Set the key
	int getTotalKeys() { return totalkeys; } // Get the keys


private:
	int keys;
	int totalkeys;
};

class Door
{
public:
	Door() = default;	// Default constructor
	~Door() = default; // Default destructor
	void DrawDoor(Tmpl8::Surface* surface, Sprites doorsprite, std::vector <HBox> doorBox); // Draw the door
	bool touchDoor(Tmpl8::Surface* surface, std::vector <HBox> doorBox, HBox playerSize, float x, float y); // Check if the player is inside the door

private:
	bool isOpen = true;
};

class Text
{
public:
	Text() = default; // Default constructor
	~Text() = default; // Default destructor
	void DrawCurrentText(Tmpl8::Surface* surface, Sprites textsprite, std::vector <HBox> textBox); // Draw the text


private:
	bool isOpen = true;
};