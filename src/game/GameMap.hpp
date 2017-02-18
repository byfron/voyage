#pragma once

#include "WorldProperties.hpp"
#include <memory>
#include <vector>
#include <Eigen/Dense>
#include <utils/GeometryUtils.hpp>
#include <common/math.hpp>
#include <utils/Configuration.hpp>
#include "tilemap.pb.h"

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
	void loadMapData(const std::string & data_file);
	void loadMaskData(const std::string & data_file);

	Vec2i getTileCoords(Vec3f pos) { return Vec2i(pos(0)/m_tile_size, pos(1)/m_tile_size);  }
	GeometryUtils::AABB getTileAABB(Vec2i tile) {
		GeometryUtils::AABB box;
		box.m_min = Vec3f(tile(0)*m_tile_size, tile(1)*m_tile_size, 0.0f);
		box.m_max = Vec3f((tile(0)+1)*m_tile_size, (tile(1)+1)*m_tile_size, 0.0f);
		return box;
	}

	CollisionMask & getCollisionMask() { return m_collision_mask; }

	pumpkin::Configuration<voyage::TileMapCfg> getConfig() {
		return m_config;
	}

	int size_x() { return m_size_x; }
	int size_y() { return m_size_y; }
	float tile_size() { return m_tile_size; }
	TileMapData & tilemap_data() { return m_tilemap_data; }
		
	typedef std::shared_ptr<GameMap> Ptr;

private:

	pumpkin::Configuration<voyage::TileMapCfg> m_config;
	CollisionMask m_collision_mask;
//	
	TileMapData m_tilemap_data;
	float m_tile_size;
	int m_size_x;
	int m_size_y;

};
