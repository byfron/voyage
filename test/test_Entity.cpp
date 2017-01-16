#include <entities/Entity.hpp>
#include <gtest/gtest.h>

class Speed {
public:
	Speed(float _s) : s(_s) {}
	float s;
};

class Pos {
public:
	Pos(float _x, float _y) : x(_x), y(_y) {}
	float x;
	float y;
};

TEST(TestEntity, Manager) {

	EntityManager em;
	Entity e1 = em.create();
	Entity e2 = em.create();

	ComponentHandle<Pos> ch1 = em.assign<Pos>(e1.id(), 1, 4.5);
	ComponentHandle<Pos> ch2 = em.assign<Pos>(e2.id(), 2, 5.5);
	em.assign<Speed>(e1.id(), 100.0);
	em.assign<Speed>(e2.id(), 120.0);
	
	ComponentHandle<Pos> ch3 = e1.component<Pos>();
	
	EXPECT_TRUE(ch1.entity().id() == e1.id());
	EXPECT_TRUE(ch2.entity().id() == e2.id());

	Pos *c1 = em.getComponentPtr<Pos>(e1.id());

	EXPECT_TRUE(ch1.get()->x == 1);
	EXPECT_TRUE(ch1.get()->y == 4.5);
	EXPECT_TRUE(c1->x == 1);
	EXPECT_TRUE(c1->y == 4.5);

	em.each<Pos,Speed>([&](Entity e, Pos &p, Speed &v) { std::cout << e.id().index() << ":" << p.x << "," << v.s << std::endl; });
	
}
