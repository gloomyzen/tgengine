#ifndef TOWERDEFENSE_GAME_SPRITECOMPONENT_H
#define TOWERDEFENSE_GAME_SPRITECOMPONENT_H

#include "AllComponentsHeaders.h"
#include "Core/ResourceModule/ResourceManager.h"
#include <map>
#include "Core/moduleDefinitions.h"

namespace TGEngine::core {

	struct sAnimation {
		int index = 0;
		int frames = 0;
		int speed = 0;

		sAnimation() = default;
	};

	class SpriteComponent : public Component {
	private:
		TransformComponent *transform{};
		SDL_Texture *texture{};
		SDL_Rect srcRect{}, destRect{};
		std::string imagePath{};

		bool mAnimated = false;
		int frames = 0;
		int speed = 100;
		int animIndex = 0;
		std::map<std::string, sAnimation *> animations;
		SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
		double angle = 0.0;

	public:
		SpriteComponent() = default;

		// TODO add width and height for image
		explicit SpriteComponent(const char *path) { setTexture(path); }

		~SpriteComponent() { SDL_DestroyTexture(texture); }

		void init() override {
			if (!getNode().hasComponent<TransformComponent>()) { getNode().addComponent<TransformComponent>(); }

			transform = &getNode().getComponent<TransformComponent>();
			srcRect.x = 0;
			srcRect.y = 0;
			srcRect.w = transform->getWidth();
			srcRect.h = transform->getHeight();
			destRect.w = static_cast<int>(static_cast<float>(transform->getWidth()) * transform->getScale());
			destRect.h = static_cast<int>(static_cast<float>(transform->getHeight()) * transform->getScale());
		}

		void update() override {
			if (isAnimated()) {
				srcRect.x =
						srcRect.w
						* static_cast<int>((SDL_GetTicks() / static_cast<unsigned int>(speed)) %
										   static_cast<unsigned int>(frames));
			}
			srcRect.y = animIndex * transform->getHeight();

			destRect.x = static_cast<int>(transform->getGlobalX());
			destRect.y = static_cast<int>(transform->getGlobalY());
			destRect.w = static_cast<int>(static_cast<float>(transform->getWidth()) * transform->getScale());
			destRect.h = static_cast<int>(static_cast<float>(transform->getHeight()) * transform->getScale());
		}

		void render() override {
			GET_TEXTURE_MANAGER()->DrawFlip(texture, srcRect, destRect, angle, spriteFlip);
		}

		void setTexture(const char *path) {
			imagePath = path;
			texture = GET_TEXTURE_MANAGER()->LoadTexture(path);
		}

		bool textureIsInited() { return texture != nullptr; }

		bool isAnimated() { return mAnimated; }

		void setAnimated(bool value) { mAnimated = value; }

		//TODO need function for play after last frame is active
		void play(const std::string &animationName) {
			if (!hasAnimation(animationName)) return;
			frames = animations[animationName]->frames;
			animIndex = animations[animationName]->index;
			speed = animations[animationName]->speed;
		}

		sAnimation *getAnimation(const std::string &animName) {
			if (hasAnimation(animName)) { return animations[animName]; }
			return nullptr;
		}

		bool hasAnimation(const std::string &animName) {
			auto currentAnimation = animations.find(animName);
			if (currentAnimation != animations.end()) { return true; }
			return false;
		}

		void addAnimation(const std::string &animName, int _index, int _frames, int _speed) {
			auto tempAnim = new sAnimation();
			tempAnim->index = _index;
			tempAnim->frames = _frames;
			tempAnim->speed = _speed;
			animations.emplace(animName, tempAnim);
		}

		void addAnimation(const std::string &animName, sAnimation *animation) {
			animations.emplace(animName, animation);
		}

		std::map<std::string, sAnimation *> &getAnimations() { return animations; }

		std::string getImagePath() { return imagePath; }
	};

}// namespace TGEngine::core

#endif// TOWERDEFENSE_GAME_SPRITECOMPONENT_H
