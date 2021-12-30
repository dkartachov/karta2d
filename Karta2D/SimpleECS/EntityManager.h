#pragma once
#include "SimpleECS.h"

class EntityManager {
public:
	static EntityManager& getInstance() {
		static EntityManager instance;

		return instance;
	}

	void addEntities(std::vector<Entity*>& entities) {
		for (const auto& entity : entities) {
			this->entities.emplace_back(entity);
		}
	}

	std::vector<Entity*>& getEntities() {
		return entities;
	}

	void update() {
		for (const auto& entity : entities) {
			entity->update();
		}
	}

	void render() {
		for (const auto& entity : entities) {
			entity->render();
		}
	}

private:
	std::vector<Entity*> entities;
};