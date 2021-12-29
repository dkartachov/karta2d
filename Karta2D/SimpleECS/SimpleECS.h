#pragma once
#include <vector>
#include <array>
#include <memory>
#include <bitset>
#include <string>

class Component;
class Entity;

constexpr std::size_t maxComponents = 32;
using ComponentBitSet = std::bitset<maxComponents>;
using ComponentArray = std::array<Component*, maxComponents>;
using ComponentId = std::size_t;

inline ComponentId getTypeId() {
	static ComponentId id = -1;
	id++;
	return id;
}

template <typename T>
inline ComponentId getTypeId() {
	static ComponentId typeId = getTypeId();
	return typeId;
}

class Component {
public:
	virtual void init() {};
	virtual void update() {};
	virtual void render() {};
	virtual void toString() {};

	void setActive(bool active) {
		this->active = active;
	}

	bool isActive() {
		return active;
	}

public:
	Entity* entity;
	bool active = true;
};

class Entity {
public:
	Entity() {
		static int id = 0;
		id++;

		this->id = id;
		name = "Entity " + std::to_string(id);
	}

	void setName(std::string name) {
		this->name = name;
	}

	std::string getName() {
		return name;
	}

	int getId() {
		return id;
	}

	bool hasChildren() {
		return children.size() > 0;
	}

	void addChildren(std::vector<Entity*> children) {
		for (const auto& child : children) {
			this->children.emplace_back(child);
		}
	}

	Entity* getChildById(int id) {
		auto it = std::find_if(children.begin(), children.end(), [&id](Entity* child) { return child->getId() == id; });

		if (it != children.end()) {
			return children.at(it - children.begin());
		}

		return nullptr;
	}

	std::vector<Entity*> getChildren() {
		return children;
	}

	void update() {
		for (auto& const component : components) {
			if (component->isActive()) {
				component->update();
			}
		}
	}

	void render() {
		for (auto& const component : components) {
			if (component->isActive()) {
				component->render();
			}
		}
	}

	bool isActive() {
		return active;
	}

	template <typename T>
	void AddComponent() {
		T* component = new T();
		component->entity = this;

		std::unique_ptr<Component> uComponent{ component };
		components.emplace_back(std::move(uComponent));

		componentArray[getTypeId<T>()] = component;
		componentBitSet[getTypeId<T>()] = 1;

		component->init();
	}

	template <typename T>
	T* GetComponent() {
		Component* component = componentArray[getTypeId<T>()];
		return dynamic_cast<T*>(component);
	}

	template <typename T>
	bool HasComponent() {
		return componentBitSet[getTypeId<T>()];
	}

private:
	int id;
	std::string name;
	bool active = true;
	ComponentArray componentArray;
	ComponentBitSet componentBitSet;

	std::vector<std::unique_ptr<Component>> components;
	std::vector<Entity*> children;
};