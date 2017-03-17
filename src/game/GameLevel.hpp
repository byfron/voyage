#pragma once

#include <Eigen/Dense>
#include <utils/GeometryUtils.hpp>
#include <memory>
#include <iostream>

struct LevelRoom {

	LevelRoom() {}
	
	LevelRoom(int id,
		  std::vector<FbxLoader::FPolygon> cps,
		  std::vector<FbxLoader::FPolygon> rps) : node_id(id) {

		for (auto rp : rps)
			room_polygons.push_back(GeometryUtils::convertToPolygon(rp));

		for (auto cp : cps)  {
			collision_polygons.push_back(GeometryUtils::convertToPolygon(cp));
			std::cout << cp << std::endl;
		}
		getchar();
	}

	int node_id;
	std::vector<GeometryUtils::Polygon> room_polygons;
	std::vector<GeometryUtils::Polygon> collision_polygons;
};

class GameLevel {

public:
	
	void loadFromFbxFile(const std::string & file);
	
	bool getRoomFromPos(const Vec3f & pos, LevelRoom & roompos) const {

		// search which polygon pos falls in
		for (auto room : m_rooms) {
			if (GeometryUtils::isWithin(pos, room.room_polygons)) {
				roompos = room;
				return true;
			}			
		}

		return false;
	}
	
	std::vector<LevelRoom> getAdjacentRooms(const LevelRoom & levelRoom) const {
		std::vector<LevelRoom> adj_rooms;
		adj_rooms.push_back(levelRoom);
		return adj_rooms;
	}
	
	std::vector<GeometryUtils::Polygon> getCollisionPolygonsNearby(const Vec3f & pos) {

		// check in which room the player is
		LevelRoom roomInPos;
		getRoomFromPos(pos, roomInPos);

		std::cout << "roomInPos:" << m_rooms[0].collision_polygons.size();
		
		std::vector<LevelRoom> rooms = getAdjacentRooms(roomInPos);

		std::vector<GeometryUtils::Polygon> collision_polygons;		
		for (auto r : rooms) {
			std::cout << "room pols" << r.collision_polygons.size() << std::endl;
			for (auto pol : r.collision_polygons) {
				collision_polygons.push_back(pol);
			}
		}
		
		return collision_polygons;
	}

	typedef std::shared_ptr<GameLevel> Ptr;

	GameLevel(const std::string &);

private:

	std::vector<LevelRoom> m_rooms;
	
	//Camera

	//Lights

	//?

};
