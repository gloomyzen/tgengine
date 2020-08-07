#include "NodeFactory.h"
#include "../Components/AllComponentsHeaders.h"

using namespace TGEngine::core;

std::map<std::string, eNodeFactory> componentsMap = {
  {"TransformComponent", eNodeFactory::TRANSFORM_COMPONENT},
  {"SpriteComponent", eNodeFactory::SPRITE_COMPONENT},
  {"AnimspriteComponent", eNodeFactory::ANIMSPRITE_COMPONENT},
};

void NodeFactory::getComponents(Node *node, const std::string &componentName) {
  if (node == nullptr)
    return;

  if (!hasRegisteredComponent(componentName)) {
      //TODO LOG_ERROR!!!
      return;
  }

  eNodeFactory needle = componentsMap[componentName];

  switch (needle) {
    case TRANSFORM_COMPONENT:
        node->addComponent<TransformComponent>();
        break;
    case SPRITE_COMPONENT:
        node->addComponent<SpriteComponent>();
        break;
    case ANIMSPRITE_COMPONENT:
        node->addComponent<SpriteComponent>();
        break;
  }
}

bool NodeFactory::hasRegisteredComponent(const std::string &componentName) {
    return (componentsMap.find(componentName) != componentsMap.end());
}
