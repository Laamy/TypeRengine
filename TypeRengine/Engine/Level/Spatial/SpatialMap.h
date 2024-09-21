#ifndef SPATIAL_MAP_H
#define SPATIAL_MAP_H

#include "SFML/Graphics.hpp"

#include <unordered_map>
#include <unordered_set>
#include <mutex>

#include "../../Objects/SolidActor.h"

// the hash function im using for my spatial partioning grid
#define Hash(x, y) x + y * 73856093

// NOTE: move these to a utils class
template<typename K, typename V>
__forceinline bool contains_value(const std::unordered_map<K, V>& map, const V& value) {
	for (const auto& pair : map) {
		if (pair.second == value)
			return true;
	}

	return false;
}

template<typename K, typename V>
__forceinline bool contains_key(const std::unordered_map<K, V>& map, const K& key) {
	for (const auto& pair : map) {
		if (pair.first == key)
			return true;
	}

	return false;
}

// NOTE: make some kind of extension (macro) for this and put into utils
__forceinline bool Intersects(const sf::FloatRect first, const sf::FloatRect other) {
	return !(first.left > other.left + other.width ||
		first.left + first.width < other.left ||
		first.top > other.top + other.height ||
		first.top + first.height < other.top);
}

class SpatialMap
{
private:
	std::size_t CellSize;

	std::unordered_map<int, std::vector<SolidActor*>> hashGrid;
	std::unordered_map<SolidActor*, std::vector<int>> objectsToHashMap;

	std::mutex gridMutex; //concurrency sake
	
	std::size_t Count;
	std::size_t Hashes;

public:
	__forceinline size_t GetCount() { return Count; }
	__forceinline size_t GetHashes() { return Hashes; }

	SpatialMap(std::size_t cellSize = 50) {
		this->CellSize = cellSize;
	}

	__forceinline static int HashPosition(std::size_t CellSize, sf::Vector2f position) {
		return HashPosition(CellSize, position.x, position.y);
	}

	__forceinline static int HashPosition(std::size_t CellSize, float iX, float iY) {
		int x = (int)std::floor(iX / CellSize);
		int y = (int)std::floor(iY/ CellSize);

		return Hash(x, y);
	}

	__forceinline std::vector<int> GetHashesForBounds(sf::FloatRect bounds) {
		std::vector<int> hashes; // NOTE: replace this with better data structure

		int index = 0;

		int minX = (int)std::floor(bounds.left / this->CellSize);
		int maxX = (int)std::floor((bounds.left + bounds.width) / this->CellSize);
		int minY = (int)std::floor(bounds.top / this->CellSize);
		int maxY = (int)std::floor((bounds.top + bounds.height) / this->CellSize);

		for (size_t x = minX; x <= maxX; ++x)
		{
			for (size_t y = minY; y <= maxY; ++y)
			{
				hashes.push_back(HashPosition(this->CellSize, x, y));
			}
		}

		return hashes;
	}

	__forceinline void AddObject(SolidActor* obj) {
		auto Shape = obj->GetShape();

		// TODO: ENTT position and size
		auto bounds = sf::FloatRect(Shape->Position, Shape->Size);
		auto numHashes = GetHashCount(bounds);
		std::vector<int> objectHashes = GetHashesForBounds(bounds);

		bool added = false;

		for (auto hash : objectHashes) {
			if (!contains_key(hashGrid, hash)) {
				hashGrid[hash] = std::vector<SolidActor*>();
				Hashes++;
			}
			
			hashGrid[hash].push_back(obj);
			added = true;
		}

		if (added) {
			Count += obj->GetCount()->ObjectCount;
			objectsToHashMap[obj] = objectHashes;
		}
	}

	__forceinline void RemoveObject(SolidActor* obj) {
		auto it = objectsToHashMap.find(const_cast<SolidActor*>(obj));
		if (it == objectsToHashMap.end())
			return;

		std::vector<int> objectHashes = it->second;

		for (const auto& hash : objectHashes) {
			auto gridIt = hashGrid.find(hash);
			if (gridIt != hashGrid.end()) {
				auto& objVec = gridIt->second;
				objVec.erase(std::remove(objVec.begin(), objVec.end(), const_cast<SolidActor*>(obj)), objVec.end());

				if (objVec.empty()) {
					hashGrid.erase(gridIt);
					Hashes--;
				}
			}
		}

		objectsToHashMap.erase(it);

		Count -= obj->GetCount()->ObjectCount;
	}

	/// <summary>
	/// DO NOT USE THIS FUNCTION! IT IS SO FUCKING SLOW (use it for networking objects only !)
	/// </summary>
	__forceinline void UpdateObject(SolidActor* obj) {
		RemoveObject(obj);
		AddObject(obj);
	}

	__forceinline int GetHashCount(const sf::FloatRect& bounds) {
		int minX = static_cast<int>(std::floor(bounds.left / CellSize));
		int maxX = static_cast<int>(std::floor((bounds.left + bounds.width) / CellSize));
		int minY = static_cast<int>(std::floor(bounds.top / CellSize));
		int maxY = static_cast<int>(std::floor((bounds.top + bounds.height) / CellSize));

		return (maxX - minX + 1) * (maxY - minY + 1);
	}

	__forceinline std::vector<SolidActor*> GetObjectsInBounds(const sf::FloatRect& bounds) {
		std::unordered_set<SolidActor*> seenObjects;
		std::vector<SolidActor*> result{};

		int numHashes = GetHashCount(bounds);
		std::vector<int> hashes(numHashes);

		hashes = GetHashesForBounds(bounds);

		for (const auto& hash : hashes) {
			std::lock_guard<std::mutex> lock(gridMutex); // Lock the grid for thread safety
			auto gridIt = hashGrid.find(hash);

			if (gridIt != hashGrid.end()) {
				auto& objects = gridIt->second;

				for (auto* obj : objects) {
					auto Shape = obj->GetShape();

					sf::FloatRect objBounds(Shape->Position.x, Shape->Position.y, Shape->Size.x, Shape->Size.y);

					if (seenObjects.find(obj) == seenObjects.end() && Intersects(objBounds, bounds)) {
						seenObjects.insert(obj);
						result.push_back(obj);
					}
				}
			}
		}

		return result;
	}

	__forceinline std::vector<SolidActor*> GetNearbyObjects(const sf::Vector2f& position, float radius) {
		sf::FloatRect bounds(position.x - radius, position.y - radius, radius * 2, radius * 2);
		return GetObjectsInBounds(bounds);
	}

	__forceinline void Clear() {
		hashGrid.clear();
		objectsToHashMap.clear();

		Hashes = 0;
		Count = 0;
	}
};

#endif // SPATIAL_MAP_H