#ifndef TOWERDEFENSE_GAME_ECS_H
#define TOWERDEFENSE_GAME_ECS_H

#include <vector>
#include <memory>
#include <algorithm>
#include <bitset>
#include <array>

namespace TGEngine::core {

class Component;
class Entity;
class Manager;

/*using ComponentID = std::size_t;
using Group = std::size_t;

inline ComponentID getNewComponentTypeID()
{
  static ComponentID lastID = 0u;
  return lastID++;
}

template<typename T> inline ComponentID getComponentTypeID() noexcept
{
  static_assert(std::is_base_of<Component, T>::value, "");
  static ComponentID typeID = getNewComponentTypeID();
  return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups = 32;

using ComponentBitSet = std::bitset<maxComponents>;
using GroupBitset = std::bitset<maxGroups>;

using ComponentArray = std::array<Component *, maxComponents>;*/

/*class IComponent
{
private:
  bool active = true;

public:
  Entity *entity{};

  virtual ~IComponent() = default;

  virtual void init() {}
  virtual void update() {}
  virtual void render() {}
  void destroy() { active = false; }

  bool &getActive() { return active; };
  bool isActive() const { return active; };
  void setActive(bool _active) { active = _active; };
};

class Component : public IComponent
{
public:
  virtual void init() {}
  virtual void update() {}
  virtual void render() {}
  virtual ~Component() = default;
};*/

class Entity
{
private:
  Manager &manager;
  bool active = true;
  std::vector<std::unique_ptr<Component>> components;

  ComponentArray componentArray{};
  ComponentBitSet componentBitSet;
  GroupBitset groupBitSet;

public:
  explicit Entity(Manager &mManager) : manager(mManager) {}

  void update()
  {
    for (auto &c : components) {
      if (c->isActive()) c->update();
    }
  }
  void render()
  {
    for (auto &c : components) {
      if (c->isActive()) c->render();
    }
  }

  bool hasGroup(Group mGroup) { return groupBitSet[mGroup]; }

  void addGroup(Group mGroup);

  void delGroup(Group mGroup) { groupBitSet[mGroup] = false; }

  template<typename T>[[nodiscard]] bool hasComponent() const { return componentBitSet[getComponentTypeID<T>()]; }

  template<typename T, typename... TArgs> T &addComponent(TArgs &&... mArgs)
  {
    T *c(new T(std::forward<TArgs>(mArgs)...));
    c->entity = this;
    std::unique_ptr<Component> uPtr{ c };
    components.emplace_back(std::move(uPtr));

    componentArray[getComponentTypeID<T>()] = c;
    componentBitSet[getComponentTypeID<T>()] = true;

    c->init();
    return *c;
  }

  template<typename T> T &getComponent() const
  {
    auto ptr(componentArray[getComponentTypeID<T>()]);
    return *static_cast<T *>(ptr);
  }
};

class Manager
{
private:
  std::vector<std::unique_ptr<Entity>> entities;
  std::array<std::vector<Entity *>, maxGroups> groupedEntities;

public:
  void update()
  {
    for (auto &e : entities) e->update();
  }
  void render()
  {
    for (auto &e : entities) e->render();
  }
  void refresh()
  {
    for (auto i(0u); i < maxGroups; i++) {
      auto &v(groupedEntities[i]);
      v.erase(
        std::remove_if(
          std::begin(v), std::end(v), [i](Entity *mEntity) { return !mEntity->isActive() || !mEntity->hasGroup(i); }),
        std::end(v));
    }

    entities.erase(std::remove_if(std::begin(entities),
                     std::end(entities),
                     [](const std::unique_ptr<Entity> &mEntity) { return !mEntity->isActive(); }),
      std::end(entities));
  }

  void addToGroup(Entity *mEntity, Group mGroup) { groupedEntities[mGroup].emplace_back(mEntity); }

  std::vector<Entity *> &getGroup(Group mGroup) { return groupedEntities[mGroup]; }

  Entity &addEntity()
  {
    auto *e = new Entity(*this);
    std::unique_ptr<Entity> uPtr{ e };
    entities.emplace_back(std::move(uPtr));
    return *e;
  }
};

}// namespace TGEngine::core

#endif// TOWERDEFENSE_GAME_ECS_H