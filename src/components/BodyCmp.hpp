#pragma once
#include <string>
#include <common/math.hpp>
#include <utils/GeometryUtils.hpp>
#include <entities/System.hpp>
#include <voyage.pb.h>
#include <queue>

struct BodyState {

};

class BodyCmp {

public:
	BodyCmp(const float & speed = 0.0f,
		const Vec3f & motion = Vec3f::Zero(),
		const Vec3f & pos = Vec3f::Zero(),
		const Eigen::MatrixXf & rot = Eigen::MatrixXf::Identity(4,4),
		const Vec2i & tile = Vec2i::Zero() ) : m_moveSpeed(speed),
						       m_moveVec(motion),
						       m_position(pos),
						       m_rotation(rot),
						       m_rotAngle(0.0f)
	{
		m_aabb.m_min = Vec3f(-0.5, -0.25,  -0.01);
		m_aabb.m_max = Vec3f(0.0,  0.25,  0.01);
	}

	BodyCmp(const std::string & cfg/*config*/) {
		
		m_moveSpeed = 2.0; //same speed as the camera now
		m_rotAngle = 0.0;
		m_moveVec = Vec3f::Zero(),
		m_position = Vec3f(0.0f, 0.0f, -0.1f);
		m_rotation = Eigen::MatrixXf::Identity(4,4);

		// TODO set this to the real thing! now is the sprite size!
		m_aabb.m_min = Vec3f(-0.25, -0.25,  -0.01);
		m_aabb.m_max = Vec3f(0.25,  0.25,  0.01);
	}

	void updateRotation() {

		Eigen::AngleAxis<float> aa(m_rotAngle, Vec3f(0.0,0.0,1.0));
		Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
		rot.block(0,0,3,3) = aa.matrix();
			
		m_rotation = rot;
	}


	GeometryUtils::Polygon getPolygon() {

		Eigen::Matrix<float, 4,4> points = Eigen::Matrix<float, 4,4>::Zero();
		points(0,0) = m_aabb.m_min(0);
		points(1,0) = m_aabb.m_min(1);

		points(0,1) = m_aabb.m_min(0);
		points(1,1) = m_aabb.m_max(1);

		points(0,2) = m_aabb.m_max(0);
		points(1,2) = m_aabb.m_max(1);

		points(0,3) = m_aabb.m_max(0);
		points(1,3) = m_aabb.m_min(1);

		points.row(3) = Vec4f::Ones();

		//TODO just rotate the centroid
		// This should go into CollisionComponent
		// We shoud just get the centroud of the object
		// and update the polygon there

		Eigen::MatrixXf T = getTransform();
		Eigen::Matrix<float, 4,4> points_trans = T * points;

		Vec4f centroid = points_trans.rowwise().mean();

		Vec2f pm2 = TO_2DVEC(centroid);

		GeometryUtils::Polygon p;
		p.vertex[0] = Vec2f(-0.25, -0.25) + pm2;
		p.vertex[1] = Vec2f(-0.25,  0.25) + pm2;
		p.vertex[2] = Vec2f( 0.25, -0.25) + pm2;
		p.vertex[3] = Vec2f( 0.25,  0.25) + pm2;
		return p;
	}


	void sync_with_server() {


		// try  to converge current state towards authoritative



		// For the time being, just correct it
		m_position = m_auth_position;
		m_rotAngle = m_auth_rotAngle;
		
		

	}
	
	Eigen::MatrixXf getTransform() {
		//NOTE: this could be pre-computed and updated only if dirty

		return Eigen::Affine3f(Eigen::Translation3f(m_position(0),
							    m_position(1),
							    m_position(2))).matrix() * m_rotation;
	}

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
