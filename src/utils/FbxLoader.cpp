#include "FbxLoader.hpp"
#include <iostream>

#ifdef IOS_REF
	#undef  IOS_REF
	#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
    //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
    pManager = FbxManager::Create();
    if( !pManager )
    {
        FBXSDK_printf("Error: Unable to create FBX Manager!\n");
        exit(1);
    }
	else FBXSDK_printf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "My Scene");
	if( !pScene )
    {
        FBXSDK_printf("Error: Unable to create FBX scene!\n");
        exit(1);
    }
}


void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
    //Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
    if( pManager ) pManager->Destroy();
	if( pExitStatus ) FBXSDK_printf("Program Success!\n");
}


bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor,  lSDKMinor,  lSDKRevision;
    //int lFileFormat = -1;
    int i, lAnimStackCount;
    bool lStatus;
    char lPassword[1024];

    // Get the file version number generate by the FBX SDK.
    FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pManager,"");

    // Initialize the importer by providing a filename.
    const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

    if( !lImportStatus )
    {
        FbxString error = lImporter->GetStatus().GetErrorString();
        FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
        FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

        if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
        {
            FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
            FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
        }

        return false;
    }

    FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

    if (lImporter->IsFBX())
    {
        FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

        // From this point, it is possible to access animation stack information without
        // the expense of loading the entire file.

        FBXSDK_printf("Animation Stack Information\n");

        lAnimStackCount = lImporter->GetAnimStackCount();

        FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
        FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
        FBXSDK_printf("\n");

        for(i = 0; i < lAnimStackCount; i++)
        {
            FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

            FBXSDK_printf("    Animation Stack %d\n", i);
            FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
            FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

            // Change the value of the import name if the animation stack should be imported
            // under a different name.
            FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

            // Set the value of the import state to false if the animation stack should be not
            // be imported.
            FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
            FBXSDK_printf("\n");
        }

        // Set the import states. By default, the import states are always set to
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene.
    lStatus = lImporter->Import(pScene);

    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
    {
	    FBXSDK_printf("Please enter password: ");

	    lPassword[0] = '\0';

	    FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
		    scanf("%s", lPassword);
	    FBXSDK_CRT_SECURE_NO_WARNING_END

		    FbxString lString(lPassword);

	    IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
	    IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

	    lStatus = lImporter->Import(pScene);

	    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
	    {
		    FBXSDK_printf("\nPassword is wrong, import aborted.\n");
	    }
    }

    // Destroy the importer.
    lImporter->Destroy();

    return lStatus;
}

std::vector<FbxLoader::FPolygon> getMeshPolygonsFromNode(FbxNode *pNode) {

	FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute();
	int num_vertices = pMesh->GetControlPointsCount();
	FbxVector4* controlPoints = pMesh->GetControlPoints();
	int num_polygons = pMesh->GetPolygonCount();
	std::vector<FbxLoader::FPolygon> polygons;

	for (int i = 0; i < num_polygons; i++) {

		FbxLoader::FPolygon cpo;
		cpo = FbxLoader::FPolygon::Zero(pMesh->GetPolygonSize(i), 3);
		for (int t = 0; t < pMesh->GetPolygonSize(i); t++) {
			int vertexIndex = pMesh->GetPolygonVertex(i, t);
			cpo(t,0) = controlPoints[vertexIndex][0];
			cpo(t,1) = controlPoints[vertexIndex][1];
			cpo(t,2) = controlPoints[vertexIndex][2];
		}

		polygons.push_back(cpo);
	}

	return polygons;
}

FbxLoader::FbxLoader(const std::string fbx_file) {

	// Prepare the FBX SDK.
	InitializeSdkObjects(_sdkManager, _scene);

	// The example can take a FBX file as an argument.
	FbxString lFilePath(fbx_file.c_str());

	if( lFilePath.IsEmpty() )
	{
		_result = false;
		FBXSDK_printf("\n\nUsage: ImportScene <FBX file name>\n\n");
	}
	else
	{
		FBXSDK_printf("\n\nFile: %s\n\n", lFilePath.Buffer());
		_result = LoadScene(_sdkManager, _scene, lFilePath.Buffer());
	}

	if(_result == false)
	{
		FBXSDK_printf("\n\nAn error occurred while loading the scene...");
		DestroySdkObjects(_sdkManager, _result);
	}
}

FbxLoader::~FbxLoader() {
	DestroySdkObjects(_sdkManager, _result);
}

std::vector<int> FbxLoader::getRoomNodeIds() const {
	std::vector<int> room_ids;
	room_ids.push_back(0);
	return room_ids;
}

std::vector<FbxLoader::FPolygon> FbxLoader::loadRoomPolygons(int node_id) const {

	// FbxNode* pRootNode = _scene->GetRootNode();
	// FbxNode* pRoomNode = pRootNode->GetChild(node_id);
	// return getMeshPolygonsFromNode(pRoomNode->GetChild(1));
	return std::vector<FbxLoader::FPolygon>();
}

std::vector<FbxLoader::FPolygon> FbxLoader::loadRoomCollisionPolygons(int node_id) const {

	FbxNode* pRootNode = _scene->GetRootNode();
	FbxNode* pRoomNode = pRootNode->GetChild(node_id);

	return getMeshPolygonsFromNode(pRoomNode->GetChild(0));

}
