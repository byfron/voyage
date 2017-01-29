#pragma once
#include <string>
#include <common/math.hpp>

class BodyCmp {
public:
	BodyCmp() {
	}
	
	BodyCmp(const std::string & cfg/*config*/) {
		m_moveSpeed = 5.0; //same speed as the camera now
		m_position = Vec3f(0.0f, 1.5f, 0.0f);
		m_tile_pos = Vec2i(0,0);
		m_action_id = 0;
	} 
public:

	float m_moveSpeed;
	Vec3f m_position;
	Vec2i m_tile_pos;

	// This is used as an internal (user-data) state
	// to ease communication with animation or other system
	// An alternative to this would be to dispatch an event
	int32_t m_action_id;
};
