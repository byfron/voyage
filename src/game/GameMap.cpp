#include "GameMap.hpp"
#include <utils/Reader.hpp>
#include <utils/Configuration.hpp>
#include <graphics/TileMapLayer.hpp>

GameMap::GameMap(const std::string & cfg_file) : m_config(cfg_file) {

	m_size_x = m_config.config().size_x();
	m_size_y = m_config.config().size_y();
	m_tile_size = 0.5; //scale
	m_tilemap_data.resize(m_size_x, m_size_y);

	// Load map data
	loadMapData(std::string(CONFIG_FILE_PATH) + m_config.config().data_file());
	loadMaskData(std::string(CONFIG_FILE_PATH) + m_config.config().mask_file());
};

void GameMap::loadMapData(const std::string & data_file) {

	Eigen::Matrix<uint32_t, Eigen::Dynamic, Eigen::Dynamic> type_matrix =
		Reader::loadMatrixFromTextFile<uint32_t>(data_file, m_size_x, m_size_y);

	for (int i = 0; i < m_size_x; i++) {
		for (int j = 0; j < m_size_y; j++) {
			m_tilemap_data(i,j).type = type_matrix(i,j);
			m_tilemap_data(i,j).height = 0.0;
		}
	}
}

void GameMap::loadMaskData(const std::string & data_file) {

	m_collision_mask.m_mask =
		Reader::loadMatrixFromTextFile<uint32_t>(data_file, m_size_x, m_size_y);
}
