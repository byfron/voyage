/*
 * Copyright 2011-2017 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <common/common.h>
#include <common/bgfx_utils.h>
#include <common/entry/cmd.h>
#include <common/entry/input.h>
#include <common/camera.h>
#include <common/debugdraw/debugdraw.h>
#include "fbx_loader.hpp"
#include <bx/uint32_t.h>
#include <vector>
#include <assert.h>
#include <utils/VertexUtils.hpp>
#include <iostream>


static uint32_t s_colors[] =
{
	 0xff000000,
	 0xff0000ff,
	 0xff00ff00,
	 0xff0ff000,
	 0xffff0000,
};

static pumpkin::PosColorVertex s_polygon[] =
{
	// {0., 0., 0., 0xff000000},
	// {0., 1., 0., 0xff0000ff},
	// {1.,2., 0., 0xff00ff00},
	// {4.,2., 0., 0xff00ffff},
	// {5.,1., 0., 0xffff0000},
	// {5., 0., 0., 0xffffff00},

	{5.19695,2.54093,0, 0xff000000},
	{5.60077,1.8069,0, 0xff000000},
	{4.68606,2.24916,0, 0xff000000},
	{4.69342,2.28636,0, 0xff000000},
	{4.78544,2.59136,0, 0xff000000},
	{5.12509,2.61619,0, 0xff000000},

};

static const uint16_t s_polyTriList[] =
{
	0,
	1,
	2,
	0,
	2,
	3,
	0,
	3,
	4,
	0,
	4,
	5

};

void imageCheckerboard(void* _dst, uint32_t _width, uint32_t _height, uint32_t _step, uint32_t _0, uint32_t _1)
{
	uint32_t* dst = (uint32_t*)_dst;
	for (uint32_t yy = 0; yy < _height; ++yy)
	{
		for (uint32_t xx = 0; xx < _width; ++xx)
		{
			uint32_t abgr = ( (xx/_step)&1) ^ ( (yy/_step)&1) ? _1 : _0;
			*dst++ = abgr;
		}
	}
}

class FbxViewer : public entry::AppI
{
	void init(int _argc, char** _argv) BX_OVERRIDE
	{
		Args args(_argc, _argv);

		m_width  = 1280;
		m_height = 720;
		m_debug  = BGFX_DEBUG_TEXT;
		m_reset  = BGFX_RESET_VSYNC | BGFX_RESET_MSAA_X16;

		bgfx::init(args.m_type, args.m_pciId);
		bgfx::reset(m_width, m_height, m_reset);

		// Enable m_debug text.
		bgfx::setDebug(m_debug);

		// Set view 0 clear state.
		bgfx::setViewClear(0
				, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
				, 0x303030ff
				, 1.0f
				, 0
				);

		m_timeOffset = bx::getHPCounter();

		cameraCreate();

		const float initialPos[3] = { 0.0f, 0.0f, -5.0f };
		cameraSetPosition(initialPos);
		cameraSetVerticalAngle(0.0f);

		ddInit();

		uint8_t data[32*32*4];
		imageCheckerboard(data, 32, 32, 4, 0xff808080, 0xffc0c0c0);

		m_sprite = ddCreateSprite(32, 32, data);

		s_texColor  = bgfx::createUniform("s_texColor",  bgfx::UniformType::Int1);


			// Prepare the FBX SDK.
		InitializeSdkObjects(_sdkManager, _scene);
		// Load the scene.

		// The example can take a FBX file as an argument.
		FbxString lFilePath("");
		for( int i = 1, c = _argc; i < c; ++i )
		{
			if( FbxString(_argv[i]) == "-test" ) gVerbose = false;
			else if( lFilePath.IsEmpty() ) lFilePath = _argv[i];
		}

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
			shutdown();
		}
		else
		{
			FBXSDK_printf("\n\nDONE");

			generateScene();
		}
	}

	virtual int shutdown() BX_OVERRIDE
	{
		ddDestroy(m_sprite);

		ddShutdown();

		cameraDestroy();

		bgfx::destroyDynamicIndexBuffer(m_ibh);
		bgfx::destroyDynamicVertexBuffer(m_vbh);
		bgfx::destroyProgram(m_program);
		bgfx::destroyTexture(m_textureColor);
		bgfx::destroyUniform(s_texColor);

		// Shutdown bgfx.
		bgfx::shutdown();

		// Destroy all objects created by the FBX SDK.
		DestroySdkObjects(_sdkManager, _result);

		return 0;
	}


	void generateScene() {

		FbxNode* pRootNode = _scene->GetRootNode();
		if(pRootNode)
		{
			for(int i = 0; i < pRootNode->GetChildCount(); i++)
			{
				FbxNode* pNode = pRootNode->GetChild(i);
				if(pNode->GetNodeAttribute() == NULL)
				{
					FBXSDK_printf("NULL Node Attribute\n\n");
				}
				else {

					FbxNodeAttribute::EType attributeType =
						(pNode->GetNodeAttribute()->GetAttributeType());

					switch (attributeType)
					{
					case FbxNodeAttribute::eMesh:
						generateMesh(pNode);
						break;
					default:
						assert(false);
					}
				}

			}
		}
	}

	void generateMesh(FbxNode* pNode) {

		FbxMesh* pMesh = (FbxMesh*) pNode->GetNodeAttribute();
		int num_vertices = pMesh->GetControlPointsCount();
		FbxVector4* controlPoints = pMesh->GetControlPoints();
		int num_triangles = pMesh->GetPolygonCount();

		// TODO: texture should be in the mesh.
		// Load diffuse texture.
		m_textureColor = loadTexture("textures/floor.ktx");

		// Layer 0?
		FbxGeometryElementUV* leUV = pMesh->GetElementUV(0);

		assert(pMesh->GetElementUVCount() == 1);

		for (int i = 0; i < num_vertices; i++)
		{

			pumpkin::PosNormalTexCoordVertex p;
			p.m_x = controlPoints[i][0];
			p.m_y = controlPoints[i][1];
			p.m_z = controlPoints[i][2];
			p.m_normal = pumpkin::NORMAL_POSZ;

			FbxVector2 uvCoord = leUV->GetDirectArray().GetAt(i);

			std::cout << p.m_x << "," << p.m_y << "," << uvCoord[0] << "," << uvCoord[1] << std::endl;

			p.m_u = uvCoord[0]*0x7fff;
			p.m_v = uvCoord[1]*0x7fff;

			m_scene_vertices.push_back(p);
		}

		for (int i = 0; i < num_triangles; i++) {
			assert(pMesh->GetPolygonSize(i) == 3);
			for (int t = 0; t < pMesh->GetPolygonSize(i); t++) {
				int vertexIndex = pMesh->GetPolygonVertex(i, t);
				m_scene_triangles.push_back(vertexIndex);
			}
		}

		// for (auto v : m_scene_vertices) {
		// 	std::cout << v.m_x << "," << v.m_y << "," << v.m_z << std::endl;
		// }

		// for (auto idx : m_scene_triangles) {
		// 	std::cout << idx << std::endl;
		// }

		pumpkin::PosNormalTexCoordVertex::init();
		m_program = loadProgram("vs_test", "fs_test");

		// 	// Create static vertex buffer.
		// m_vbh = bgfx::createVertexBuffer(
		// 		// Static data can be passed with bgfx::makeRef
		// 		bgfx::makeRef(s_polygon, sizeof(s_polygon) )
		// 		, pumpkin::PosColorVertex::ms_decl
		// 		);

		// // Create static index buffer.
		// m_ibh = bgfx::createIndexBuffer(
		// 		// Static data can be passed with bgfx::makeRef
		// 		bgfx::makeRef(s_polyTriList, sizeof(s_polyTriList) )
		// 		);

		// return;

		const bgfx::Memory* mem;
		mem = bgfx::makeRef(&m_scene_vertices[0], sizeof(pumpkin::PosNormalTexCoordVertex)*m_scene_vertices.size());
		m_vbh = bgfx::createDynamicVertexBuffer(mem, pumpkin::PosNormalTexCoordVertex::ms_decl);
		mem = bgfx::makeRef(&m_scene_triangles[0], sizeof(uint16_t) * m_scene_triangles.size() );
		m_ibh = bgfx::createDynamicIndexBuffer(mem);

		// // create static vertex buffer.
		// m_vbh = bgfx::createVertexBuffer(
		// 	// Static data can be passed with bgfx::makeRef
		// 	bgfx::makeRef(&scene_vertices[0], num_vertices*sizeof(pumpkin::PosColorVertex))
		// 	, pumpkin::PosColorVertex::ms_decl
		// 	);

		// // Create static index buffer.
		// m_ibh = bgfx::createIndexBuffer(
		// 	// Static data can be passed with bgfx::makeRef
		// 	bgfx::makeRef(&scene_triangles[0], num_triangles * 3 * sizeof(uint16_t))
		// 	);


	}

	bool update() BX_OVERRIDE
	{
		if (!entry::processEvents(m_width, m_height, m_debug, m_reset, &m_mouseState) )
		{
			int64_t now = bx::getHPCounter() - m_timeOffset;
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency() );
			const double toMs = 1000.0/freq;
			const float deltaTime = float(frameTime/freq);

			// Use debug font to print information about this example.
			bgfx::dbgTextClear();
			bgfx::dbgTextPrintf(0, 1, 0x4f, "FBX viewer");
			bgfx::dbgTextPrintf(0, 2, 0x6f, "Description: Visualizer of FBX files.");
			bgfx::dbgTextPrintf(0, 3, 0x0f, "Frame: % 7.3f[ms]", double(frameTime)*toMs);

			// Update camera.
			cameraUpdate(deltaTime, m_mouseState);

			float view[16];
			cameraGetViewMtx(view);

			float proj[16];

			bx::mtxProj(proj, 60.0f, float(m_width)/float(m_height), 0.1f, 100.0f);

			bgfx::setViewTransform(0, view, proj);
			bgfx::setViewRect(0, 0, 0, m_width, m_height);

			float zero[3] = {};

			float mvp[16];
			float eye[] = { 5.0f, 10.0f, 5.0f };
			bx::mtxLookAt(view, eye, zero);
			bx::mtxProj(proj, 45.0f, float(m_width)/float(m_height), 1.0f, 15.0f);
			bx::mtxMul(mvp, view, proj);

			ddDrawOrb(0.0f, 0.0f, 0.0f, 1.0f);

			// bgfx::setState(0 |
			// 			   BGFX_STATE_DEFAULT |
			// 			   BGFX_STATE_CULL_CCW);
			// 			   | BGFX_STATE_DEFAULT
			// 			   | BGFX_STATE_PT_TRISTRIP
			// 	);


			// show mesh

			bgfx::setVertexBuffer(m_vbh);
			bgfx::setIndexBuffer(m_ibh);
			// Bind textures.
			bgfx::setTexture(0, s_texColor,  m_textureColor);

			bgfx::submit(0, m_program);

			// Advance to next frame. Rendering thread will be kicked to
			// process submitted rendering primitives.
			bgfx::frame();

			return true;
		}

		return false;
	}

	entry::MouseState m_mouseState;
	SpriteHandle m_sprite;

	int64_t m_timeOffset;

	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_debug;
	uint32_t m_reset;

	FbxManager* _sdkManager = NULL;
	FbxScene* _scene = NULL;
	bool _result;

	std::vector<pumpkin::PosNormalTexCoordVertex> m_scene_vertices;
	std::vector<uint16_t> m_scene_triangles;

	bgfx::DynamicVertexBufferHandle m_vbh;
	bgfx::DynamicIndexBufferHandle m_ibh;
	bgfx::UniformHandle s_texColor;
	bgfx::TextureHandle m_textureColor;
	bgfx::ProgramHandle m_program;
};

ENTRY_IMPLEMENT_MAIN(FbxViewer);
