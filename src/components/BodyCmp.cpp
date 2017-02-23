#include "BodyCmp.hpp"

void BodySystem::update(EntityManager & em, EventManager &evm, float delta ) {

	em.each<BodyCmp>([delta](Entity entity, BodyCmp & body) {
				
			body.m_position += body.m_moveVec;
			
			Eigen::AngleAxis<float> aa(body.m_rotAngle, Vec3f(0.0,0.0,1.0));
			Eigen::MatrixXf rot = Eigen::MatrixXf::Identity(4,4);
			rot.block(0,0,3,3) = aa.matrix();
			
			body.m_rotation = rot;

		});
}
