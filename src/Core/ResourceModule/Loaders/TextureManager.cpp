#include "TextureManager.h"
#include "Core/GameApplication.h"
#include "Core/Debug/Logger.h"

using namespace TGEngine::core;

const std::string spriteFolder = "sprites/";

std::map<std::string, SDL_Texture *> texturesMap{};

TextureManager::TextureManager() {}

TextureManager::~TextureManager() {
	clearStorage();
}

SDL_Texture *TextureManager::LoadTexture(const char *filePath) {
	std::string fullPath = spriteFolder + filePath;
	if (hasLoadedTexture(fullPath)) {
		return texturesMap[fullPath];
	}
	SDL_Surface *tempSurface = IMG_Load(fullPath.c_str());
	SDL_Texture *texture = SDL_CreateTextureFromSurface(GET_APPLICATION().getRenderer(), tempSurface);

	SDL_FreeSurface(tempSurface);

	if (texture == nullptr) {
		LOG_ERROR("ResourceManager::LoadTexture Texture '" + fullPath + "' not loaded!");
	}

	if (!hasLoadedTexture(fullPath)) {
		texturesMap.insert({fullPath, texture});
	}

	return texture;
}

void TextureManager::Draw(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest) {
	SDL_RenderCopy(GET_APPLICATION().getRenderer(), tex, &src, &dest);
}

void TextureManager::DrawFlip(SDL_Texture *tex, SDL_Rect src, SDL_Rect dest, double angle, SDL_RendererFlip flip) {
	SDL_RenderCopyEx(GET_APPLICATION().getRenderer(), tex, &src, &dest, angle, nullptr, flip);
}

bool TextureManager::hasLoadedTexture(std::string filePath) {
	return texturesMap.find(filePath) != texturesMap.end();
}

void TextureManager::clearStorage() {
	texturesMap.clear();
}
