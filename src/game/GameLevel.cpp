#include "GameLevel.hpp"
#include <utils/FbxLoader.hpp>


void GameLevel::load(const FbxLoader & loader) {

	for(int id : loader.getRoomNodeIds()) {
		std::vector<FbxLoader::FPolygon> room_polygons =
			loader.loadRoomPolygons(id);

		std::vector<FbxLoader::FPolygon> collision_polygons =
			loader.loadRoomCollisionPolygons(id);

		m_rooms.push_back(LevelRoom(id, room_polygons, collision_polygons));
	}
}
