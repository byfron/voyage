#pragma once
#include <string>
#include <common/math.hpp>
#include <utils/GeometryUtils.hpp>
#include <entities/System.hpp>
#include <voyage.pb.h>
#include <queue>

struct BodyState {

};

/**
 Main component for any object location and motion
 Client-Server
 */
class BodyComponent {

public:
	BodyComponent(const float & speed = 0.0f,
				  const Vec3f & motion = Vec3f::Zero(),
				  const Vec3f & pos = Vec3f::Zero(),
				  const Eigen::MatrixXf & rot = Eigen::MatrixXf::Identity(4,4),
				  const Vec2i & tile = Vec2i::Zero());

	BodyComponent(const std::string & cfg/*config*/);
	void updateRotation();
	GeometryUtils::Polygon getPolygon();
	void sync_with_server();
	Eigen::MatrixXf getTransform();

public:

	GeometryUtils::AABB m_aabb;

	float m_moveSpeed;
	Vec3f m_moveVec;
	Vec3f m_position;
	
	Eigen::MatrixXf m_rotation;
	float m_rotAngle;

	// Values requested by the server
	Vec3f m_auth_position;
	float m_auth_rotAngle;


	std::queue<BodyState> m_state_queue;

	// This is used as an internal (user-data) state
	// to ease communication with animation or other system
	// An alternative to this would be to dispatch an event
	// int32_t m_action_id;
};

class BodySystem : public System<BodySystem> {

	void update(EntityManager & em, EventManager &evm, float delta );
};
