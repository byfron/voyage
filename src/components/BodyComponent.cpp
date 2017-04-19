#include "BodyComponent.hpp"

BodyComponent::BodyComponent(const float & speed,
							 const Vec3f & motion,
							 const Vec3f & pos,
							 const Eigen::MatrixXf & rot,
							 const Vec2i & tile) : m_moveSpeed(speed),
												   m_moveVec(motion),
												   m_position(pos),
												   m_rotation(rot),
												   m_rotAngle(0.0f)
{
	m_aabb.m_min = Vec3f(-0.5, -0.25,  -0.01);
	m_aabb.m_max = Vec3f(0.0,  0.25,  0.01);
}

BodyComponent::BodyComponent(const std::string & cfg/*config*/) {
		
	m_moveSpeed = 2.0; //same speed as the camera now
	m_rotAngle = 0.0;
	m_moveVec = Vec3f::Zero(),
		m_position = Vec3f(0.0f, 0.0f, -0.1f);
	m_rotation = Eigen::MatrixXf::Identity(4,4);

	// TODO set this to the real thing! now is the sprite size!
	m_aabb.m_min = Vec3f(-0.25, -0.25,  -0.01);
	m_aabb.m_max = Vec3f(0.25,  0.25,  0.01);
}

void BodyComponent::updateRotation() {

	Eigen::AngleAxis<float> aa(m_rotAngle, Vec3f(0.0,0.0,1.0));
	Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
	rot.block(0,0,3,3) = aa.matrix();
			
	m_rotation = rot;
}


GeometryUtils::Polygon BodyComponent::getPolygon() {

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
	p.vertices.push_back(Vec2f(-0.25, -0.25) + pm2);
	p.vertices.push_back(Vec2f(-0.25,  0.25) + pm2);
	p.vertices.push_back(Vec2f( 0.25, -0.25) + pm2);
	p.vertices.push_back(Vec2f( 0.25,  0.25) + pm2);
	return p;
}


void BodyComponent::sync_with_server() {


	// try  to converge current state towards authoritative



	// For the time being, just correct it
	m_position = m_auth_position;
	m_rotAngle = m_auth_rotAngle;
		
		

}
	
Eigen::MatrixXf BodyComponent::getTransform() {
	//NOTE: this could be pre-computed and updated only if dirty

	return Eigen::Affine3f(Eigen::Translation3f(m_position(0),
												m_position(1),
												m_position(2))).matrix() * m_rotation;
}

void BodySystem::update(EntityManager & em, EventManager &evm, float delta ) {

	em.each<BodyComponent>([delta](Entity entity, BodyComponent & body) {
				
			body.m_position += body.m_moveVec;
			
			Eigen::AngleAxis<float> aa(body.m_rotAngle, Vec3f(0.0,0.0,1.0));
			Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
			rot.block(0,0,3,3) = aa.matrix();
			
			body.m_rotation = rot;

		});
}
