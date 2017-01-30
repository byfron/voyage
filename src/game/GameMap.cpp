#include "GameMap.hpp"
#include <utils/Reader.hpp>
#include <utils/Configuration.hpp>
#include <graphics/TileMapLayer.hpp>

GameMap::GameMap(const std::string & cfg_file) {
	pumpkin::Configuration<voyage::TileMapCfg> conf(cfg_file);
	loadTileMap(conf);
};


bool GameMap::loadTileMap(const pumpkin::Configuration<voyage::TileMapCfg> & conf) {

	m_size_x = conf.config().size_x();
	m_size_y = conf.config().size_y();
	m_tile_size = 0.5; //scale
	m_tilemap_data.resize(m_size_x, m_size_y);

	// Load map data
	loadMapData(std::string(CONFIG_FILE_PATH) + conf.config().data_file());
	loadMaskData(std::string(CONFIG_FILE_PATH) + conf.config().mask_file());

	for (int i = 0; i < conf.config().layers().size(); i++) {

		voyage::TileMapLayerCfg layer_cfg = conf.config().layers(i);

		pumpkin::TileMapLayerProperties properties(
			conf.config().size_x(),
			conf.config().size_y(),
			layer_cfg.atlas_id(),
			layer_cfg.shader_id());

		// Create Layer
		pumpkin::TileMapLayer::Ptr layer = std::make_shared<pumpkin::TileMapLayer>(properties);

		// Initialise internals before adding meshes
		layer->init();

		generateLayer(layer);

		m_tilemap.addLayer(layer);
	}

	return true;
}


void GameMap::generateLayer(pumpkin::TileMapLayer::Ptr layer) {

	float height = 0.5;
	for (int i = 0; i < m_size_x; i++) {
		for (int j = 0; j < m_size_y; j++) {

			// TODO: here we need a proper table of objects
			if (m_tilemap_data(i,j).type == 0)
				layer->addTile(i, j, m_tile_size);
			else
				layer->addWall(i, j, m_tile_size, height);
		}
	}

	// We call this after the vertices are added
	layer->initialiseBuffers();
}

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
