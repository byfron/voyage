#pragma once

#include "WorldProperties.hpp"
#include <memory>
#include <vector>
#include <Eigen/Dense>
#include <pumpkin.hpp>
#include <utils/GeometryUtils.hpp>
#include <common/math.hpp>

class MapRegion;
typedef std::shared_ptr<MapRegion> MapRegionPtr;

class WorldProperties;

struct TileProperties {
	uint32_t type;
	float height;
};

class GameMap {
public:

	typedef uint32_t CollisionData;
	typedef Eigen::Matrix<TileProperties, Eigen::Dynamic, Eigen::Dynamic> TileMapData;

	class CollisionMask {
	public:
		bool isWalkable(const Vec2i & tile) const {
			if (tile(0) >= m_mask.rows() || tile(1) >= m_mask.cols()) return true;
			if (tile(0) < 0 || tile(1) < 0) return true;
			if (m_mask(tile(0), tile(1)) == 0) return true;
			return false;
		}

		Eigen::Matrix<CollisionData, Eigen::Dynamic, Eigen::Dynamic> m_mask;
	};

	GameMap(const std::string & cfg_file);

	void generate(const WorldProperties & properties) {};
	void generateLayer(pumpkin::TileMapLayer::Ptr layer);
	bool loadTileMap(const pumpkin::Configuration<voyage::TileMapCfg> & conf);
	void loadMapData(const std::string & data_file);
	void loadMaskData(const std::string & data_file);

	Vec2i getTileCoords(Vec3f pos) { return Vec2i(pos(0)/m_tile_size, pos(1)/m_tile_size);  }
	pumpkin::TileMap & getTileMap() { return m_tilemap; }
	GeometryUtils::AABB getTileAABB(Vec2i tile) {
		GeometryUtils::AABB box;
		box.m_min = Vec3f(tile(0)*m_tile_size, tile(1)*m_tile_size, 0.0f);
		box.m_max = Vec3f((tile(0)+1)*m_tile_size, (tile(1)+1)*m_tile_size, 0.0f);
		return box;
	}

	CollisionMask & getCollisionMask() { return m_collision_mask; }

	typedef std::shared_ptr<GameMap> Ptr;

private:

	CollisionMask m_collision_mask;
	pumpkin::TileMap m_tilemap;
	TileMapData m_tilemap_data;
	float m_tile_size;
	int m_size_x;
	int m_size_y;

};
