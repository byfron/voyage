#pragma once
#include <fbxsdk.h>
#include <vector>
#include <Eigen/Dense>

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);

class FbxLoader {

public:

	typedef Eigen::Matrix<float, Eigen::Dynamic, 3> FPolygon;


	FbxLoader(const std::string fbx_file);
	~FbxLoader();

	std::vector<FPolygon> loadRoomCollisionPolygons(int id) const;
	std::vector<FPolygon> loadRoomPolygons(int id) const;
	std::vector<int> getRoomNodeIds() const;

	FbxNode * getRootNode() const { return _scene->GetRootNode(); }

private:

	FbxManager* _sdkManager = NULL;
	FbxScene* _scene = NULL;
	bool _result;

};
