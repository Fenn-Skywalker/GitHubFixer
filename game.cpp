#include "game.h"
#include "player.h"
#include "surface.h"
#include "sprites.h"
#include "layers.h"
#include "entities.h"

#include <cstdio>
#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <SDL.h>
#include <string>
#include <vector>
#include <Windows.h>
#include <chrono>

namespace Tmpl8
{
	ldtk::Project proj_ldtk; // The LDtk project
	const ldtk::Level* level = nullptr; // The current level
	const char* levelNames[] = { "Tutorial", "Lvl1", "Lvl2" , "Lvl3" , "Victory" }; // The names of the levels

	void Game::setHitboxes()
	{
		hitboxes;
	}
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------
	void Game::Init()
	{
		// Load the LDtk project
		proj_ldtk.loadFromFile("assets/mapTest.ldtk"); // Load the LDtk project from the file
		loadLevel(0); // loading the first level
		
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		std::string keysleft = "Keys Left: " + std::to_string(key.getTotalKeys() - key.getKeys()); //Prepares the string for the keys left to be collected
		screen->Clear(0xffffff);

		player.Moving(startPos); //Controlls player movement and some basic physics
		player.addCollisions(hitboxes); // Add the hitboxes to the player class and controls object collisions
		

		bgLayer.Draw(screen, { 0, 0 }); // Draw the background layer
		layer.Draw(screen, {0, 0}); // Draw the floor layer
		key.DrawKeys(screen, keys, player.GetPosition(), playerSize, keysprite); //draws the keys
		door.DrawDoor(screen, doorsprite, doorBox); // Draw the door
		if (currentLevel == 4)
		{
			text.DrawCurrentText(screen, textsheet->GetSprite(0), textBox); // Draw the exit text
		}
		if (currentLevel == 0)
		{
			text.DrawCurrentText(screen, textsheet->GetSprite(0), textBox); // Draw the tutorial text
		}
		player.Draw(screen, playersprite, false); // Draw the player
		screen->Print(const_cast<char*>(keysleft.c_str()), 350, 20, 0xFFFFFF); // Print the keys left to be collected at the top of the screen

		playersprite = charactersheet->GetSprite(player.currentframe()); //Gets the players sprite based on the current frame
		

		if (key.getTotalKeys() - key.getKeys() == 0) //checks if all keys have been collected
		{
			doorsprite = keysheet->GetSprite(10);//swaps to the "open door" sprite
			if (door.touchDoor(screen, doorBox, playerSize, player.GetPosition().x, player.GetPosition().y) == true) //checks if player collides with door
			{
				loadLevel(currentLevel + 1); //loads the next level
				key.resetCheck(); //resets the key check
				key.setKeys(0); //resets the keys for the next level
			}
		}
		else
		{
			doorsprite = keysheet->GetSprite(8); //swaps to the "closed door" sprite
		}

		if (GetAsyncKeyState(VK_SPACE)) //checks if the space bar is pressed
		{
			if (currentLevel == 4) //checks if its the last level
			{
				loadLevel(0); //loads the first level
				key.resetCheck(); //resets the key check
				key.setKeys(0); //resets the keys for the next level
			}
			else
			{
				loadLevel(currentLevel + 1); //loads the next level
				key.resetCheck(); //resets the key check
				key.setKeys(0); //resets the keys for the next level
			}
		}

		Sleep(25); // Sleep for 25 milliseconds to control the frame rate
	}


	void Game::loadLevel(int lvlNum) //https://ldtk.io/docs/game-dev/super-simple-export/ is where I learned how to incorporate the LDTK project within my game
	{
		auto& world = proj_ldtk.getWorld(); // Get the world from the project
		level = &world.getLevel(levelNames[lvlNum]); // Get the level from the world

		currentLevel = lvlNum; // Sets the current level to the level number

		const auto& floorLayer = level->getLayer("Floor"); // Get the floor layer from the level
		const auto& gridSize = floorLayer.getGridSize(); // Get the grid size of the layer
		const auto& tileSet = floorLayer.getTileset(); // Get the tileset of the layer

		const auto& entitiesLayer = level->getLayer("Entities"); // Get the entities layer from the level
		const auto& hitBoxLayer = entitiesLayer.getEntitiesByName("HitBox"); // Get the tileset of the layer
		const auto& keyLayer = entitiesLayer.getEntitiesByName("KeySpawn"); // Get the tileset of the layer
		const auto& doorLayer = entitiesLayer.getEntitiesByName("Door"); // Get the tileset of the layer

		const auto& backgroundLayer = level->getLayer("Background"); // Get the background layer from the level
		const auto& backgroundSize = backgroundLayer.getGridSize(); // Get the grid size of the layer
		const auto& backgroundSet = backgroundLayer.getTileset(); // Get the tileset of the layer

		if (currentLevel == 4) //checks if its the final level to load the exit text (crashes the game if not in an if)
		{
			const auto& textLayer = entitiesLayer.getEntitiesByName("Text");
			textBox.clear();
			for (const auto& entity : textLayer)
			{
				textBox.push_back({ entity.get().getPosition().x, entity.get().getPosition().y, entity.get().getSize().x, entity.get().getSize().y });
			}
			textsheet = std::make_shared<SpriteSheet>("assets/Exit Text.png", 128); // Load the text sheet
		}

		if (currentLevel == 0) //checks if its the final level to load the tutorial text (crashes the game if not in an if)
		{
			const auto& tutLayer = entitiesLayer.getEntitiesByName("Tut");
			textBox.clear();
			for (const auto& entity : tutLayer)
			{
				textBox.push_back({ entity.get().getPosition().x, entity.get().getPosition().y, entity.get().getSize().x, entity.get().getSize().y });
			}
			textsheet = std::make_shared<SpriteSheet>("assets/Tut.png", 128); // Load the text sheet
		}

		int playerstartX = 8 * entitiesLayer.getEntitiesByName("StartPos")[0].get().getGridPosition().x;
		int playerstartY = 8 * entitiesLayer.getEntitiesByName("StartPos")[0].get().getGridPosition().y;

		playerSize = { playerstartX, playerstartY, 16, 16 }; // setting the player size to 16x16
		startPosition = { playerstartX, playerstartY, 16, 16 }; // setting the start position of the player
		startPos = Tmpl8::vec2(startPosition.x, startPosition.y);
		player = Player(playerSize, 0, 0); // Create a player with a size of 16x16

		hitboxes.clear(); // destruct the hitboxes
		for (const auto& entity : hitBoxLayer) // Loop through all the entities in the layer
		{
			hitboxes.push_back({ entity.get().getPosition().x, entity.get().getPosition().y, entity.get().getSize().x, entity.get().getSize().y }); // Add the entity to the hitboxes vector
		}
		keys.clear();
		for (const auto& entity : keyLayer)
		{
			keys.push_back({ entity.get().getPosition().x, entity.get().getPosition().y, entity.get().getSize().x, entity.get().getSize().y });
		}
		doorBox.clear();
		for (const auto& entity : doorLayer)
		{
			doorBox.push_back({ entity.get().getPosition().x, entity.get().getPosition().y, entity.get().getSize().x, entity.get().getSize().y });
		}


		//loading player spritesheet
		charactersheet = std::make_shared<SpriteSheet>("assets/Blank Top Down 16x16 Character Template.png", 16);
		keysheet = std::make_shared<SpriteSheet>("assets/ohmydungeon_v1.1.png", 16);
		keysprite = keysheet->GetSprite(45); // Gets the key sprite from the sprite sheet

		auto spritesheet = std::make_shared<SpriteSheet>(proj_ldtk.getFilePath().directory() + tileSet.path, tileSet.tile_size); // Create a sprite sheet from the tileset
		layer = Layers(spritesheet, gridSize.y, gridSize.x); // Create a floorLayer with the sprite sheet and grid size

		for (const auto& tile : floorLayer.allTiles()) { // Loop through all the tiles in the floorLayer
			auto [x, y] = tile.getGridPosition(); // Get the grid position of the tile
			layer(y, x) = tile.tileId; // Set the tile at the grid position to the tile ID
		}

		auto spritesheet2 = std::make_shared<SpriteSheet>(proj_ldtk.getFilePath().directory() + backgroundSet.path, backgroundSet.tile_size); // Create a sprite sheet from the tileset
		bgLayer = Layers(spritesheet2, backgroundSize.y, backgroundSize.x); // Create a layer with the sprite sheet and grid size

		for (const auto& bgtile : backgroundLayer.allTiles()) { // Loop through all the tiles in the layer
			auto [x, y] = bgtile.getGridPosition(); // Get the grid position of the tile
			bgLayer(y, x) = bgtile.tileId; // Set the tile at the grid position to the tile ID
		}
	}

}