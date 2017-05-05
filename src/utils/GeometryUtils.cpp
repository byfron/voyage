#include "GeometryUtils.hpp"

namespace GeometryUtils {

	Polygon getQuadCentered(const Vec3f &centroid, float scale) {
		Polygon q;
		q.vertices.push_back(Vec2f(-1.0,-1.0) * scale + centroid.head(2));
		q.vertices.push_back(Vec2f( 1.0,-1.0) * scale + centroid.head(2));
		q.vertices.push_back(Vec2f( 1.0, 1.0) * scale + centroid.head(2));
		q.vertices.push_back(Vec2f(-1.0, 1.0) * scale + centroid.head(2));
		return q;
	}

	Polygon convertToPolygon(FbxLoader::FPolygon p) {
		Polygon pp;
		for (int i = 0; i < p.rows(); i++) {
			pp.vertices.push_back(Vec2f(p(i,0), p(i,1)));
		}
		return pp;
	}

	void CalculateInterval(Vec2f Axis, GeometryUtils::Polygon P, float& min, float& max)
	{
		float d = Axis.dot(P.vertices[0]);
		min = max = d;
		for(int I = 0; I < P.num_vertices(); I ++)
		{
			float d = P.vertices[I].dot(Axis);
			if (d < min)
				min = d;
			else
				if(d > max)
					max = d;
		}
	}

	bool AxisSeparatePolygons(Vec2f& Axis, GeometryUtils::Polygon A, GeometryUtils::Polygon B)
	{
		float mina, maxa;
		float minb, maxb;

		CalculateInterval(Axis, A, mina, maxa);
		CalculateInterval(Axis, B, minb, maxb);

		if (mina > maxb || minb > maxa)
			return true;

		// find the interval overlap
		float d0 = maxa - minb;
		float d1 = maxb - mina;
		float depth = (d0 < d1)? d0 : d1;

		// convert the separation axis into a push vector (re-normalise
		// the axis and multiply by interval overlap)
		float axis_length_squared = Axis.dot(Axis);

		Axis *= depth / axis_length_squared;
		return false;
	}


	bool Intersect(GeometryUtils::Polygon A, GeometryUtils::Polygon B)
	{
		int I = 0;
		for(int J = A.num_vertices()-1; I < A.num_vertices(); J = I, I++)
		{
			Vec2f E = A.vertices[I] - A.vertices[J];
			Vec2f N = Vec2f(-E(1), E(0));

			if (AxisSeparatePolygons(N, A, B))
				return false;
		}

		I = 0;
		for(int J = B.num_vertices()-1; I < B.num_vertices(); J = I, I ++)
		{
			Vec2f E = B.vertices[I] - B.vertices[J];
			Vec2f N = Vec2f(-E(1), E(0));

			if (AxisSeparatePolygons(N, A, B))
				return false;
		}

		return true;
	}

	Vec2f FindMTD(Vec2f* PushVector, int iNumVectors)
	{
		Vec2f MTD = PushVector[0];
		float mind2 = PushVector[0].dot(PushVector[0]);
		for(int I = 1; I < iNumVectors; I++)
		{
			float d2 = PushVector[I].dot(PushVector[I]);
			if (d2 < mind2)
			{
				mind2 = d2;
				MTD = PushVector[I];
			}
		}
		return MTD;
	}


	bool isWithin(const Vec3f & point, std::vector<Polygon>) {
//		assert(false);
		return true;
	}

	bool IntersectMTD(GeometryUtils::Polygon A, GeometryUtils::Polygon B, Vec2f& MTD)
	{
		// potential separation axes. they get converted into push
		Vec2f Axis[32];
		// max of 16 vertices per polygon
		int iNumAxis = 0;
		int J,I;
		for(J = A.num_vertices()-1, I = 0; I < A.num_vertices(); J = I, I ++)
		{
			Vec2f E = A.vertices[I] - A.vertices[J];
			Axis[iNumAxis++] = Vec2f(-E(1), E(0));

			if (AxisSeparatePolygons(Axis[iNumAxis-1], A, B))
				return false;
		}
		for(J = B.num_vertices()-1, I = 0; I < B.num_vertices(); J = I, I ++)
		{
			Vec2f E = B.vertices[I] - B.vertices[J];
			Axis[iNumAxis++] = Vec2f(-E(1), E(0));

			if (AxisSeparatePolygons (Axis[iNumAxis-1], A, B))
				return false;
		}

		// find the MTD among all the separation vectors
		MTD = FindMTD(Axis, iNumAxis);

		// makes sure the push vector is pushing A away from B
		Vec2f D = A.centroid() - B.centroid();
		if (D.dot(MTD) > 0.0f)
			MTD = -MTD;

		return true;
	}

	bool linePlaneIntersection(Vec3f ray, Vec3f rayOrigin, Vec3f normal,
				   Vec3f planePoint, Vec3f & contact) {

		ray.normalize();
		float d = normal.dot(planePoint);

		if (normal.dot(ray) == 0) {
			return false; // No intersection, the line is parallel to the plane
		}

		// Compute the X value for the directed line ray intersecting the plane
		float x = (d - normal.dot(rayOrigin)) / normal.dot(ray);

		// output contact point
		contact = rayOrigin + ray*x; //Make sure your ray vector is normalized
		return true;
	}

}
