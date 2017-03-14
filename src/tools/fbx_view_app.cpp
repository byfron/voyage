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

		const float initialPos[3] = { 0.0f, 2.0f, -12.0f };
		cameraSetPosition(initialPos);
		cameraSetVerticalAngle(0.0f);

		ddInit();

		uint8_t data[32*32*4];
		imageCheckerboard(data, 32, 32, 4, 0xff808080, 0xffc0c0c0);

		m_sprite = ddCreateSprite(32, 32, data);


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



		for (int i = 0; i < num_vertices; i++)
		{

			pumpkin::PosColorVertex p;
			p.m_x = controlPoints[i][0];
			p.m_y = controlPoints[i][1];
			p.m_z = controlPoints[i][2];

			int color_idx = i%2;
			p.m_abgr = s_colors[color_idx];
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

		pumpkin::PosColorVertex::init();
		m_program = loadProgram("vs_cubes", "fs_cubes");

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
		mem = bgfx::makeRef(&m_scene_vertices[0], sizeof(pumpkin::PosColorVertex)*m_scene_vertices.size());
		m_vbh = bgfx::createDynamicVertexBuffer(mem, pumpkin::PosColorVertex::ms_decl);
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

			// bgfx::setState(0
			// 			   | BGFX_STATE_DEFAULT
			// 			   | BGFX_STATE_PT_TRISTRIP
			// 	);


			// show mesh

			bgfx::setVertexBuffer(m_vbh);
			bgfx::setIndexBuffer(m_ibh);


			bgfx::submit(0, m_program);



			// ddBegin(0);
			// // ddDrawAxis(0.0f, 0.0f, 0.0f);

			// // ddPush();
			// // 	ddSetColor(0xff00ff00);

			// // 	Aabb aabb =
			// // 	{
			// // 		{  5.0f, 1.0f, 1.0f },
			// // 		{ 10.0f, 5.0f, 5.0f },
			// // 	};
			// // 	ddDraw(aabb);
			// // ddPop();

			// float time = float(now/freq);

			// // Obb obb;
			// // bx::mtxRotateX(obb.m_mtx, time);
			// // ddSetWireframe(true);
			// // ddDraw(obb);

			// // ddSetColor(0xffffffff);
			// // bx::mtxSRT(obb.m_mtx, 1.0f, 1.0f, 1.0f, 0.0f, time, 0.0f, 3.0f, 0.0f, 0.0f);
			// // ddSetWireframe(false);
			// // ddDraw(obb);

			// // ddSetTranslate(0.0f, -2.0f, 0.0f);
			// // ddDrawGrid(Axis::Y, zero, 20, 1.0f);
			// // ddSetTransform(NULL);

			// // ddDrawFrustum(mvp);

			// // ddPush();
			// // 	Sphere sphere = { { 0.0f, 5.0f, 0.0f }, 1.0f };
			// // 	ddSetColor(0xfff0c0ff);
			// // 	ddSetWireframe(true);
			// // 	ddSetLod(3);
			// // 	ddDraw(sphere);
			// // 	ddSetWireframe(false);

			// // 	ddSetColor(0xc0ffc0ff);
			// // 	sphere.m_center[0] = -2.0f;
			// // 	ddSetLod(2);
			// // 	ddDraw(sphere);

			// // 	ddSetColor(0xa0f0ffff);
			// // 	sphere.m_center[0] = -4.0f;
			// // 	ddSetLod(1);
			// // 	ddDraw(sphere);

			// // 	ddSetColor(0xffc0ff00);
			// // 	sphere.m_center[0] = -6.0f;
			// // 	ddSetLod(0);
			// // 	ddDraw(sphere);
			// // ddPop();

			// // ddSetColor(0xffffffff);

			// // ddPush();
			// // {
			// // 	float normal[3] = {  0.0f, 0.0f, 1.0f };
			// // 	float center[3] = { -8.0f, 0.0f, 0.0f };
			// // 	ddPush();
			// // 		ddSetStipple(true, 1.0f, time*0.1f);
			// // 		ddSetColor(0xff0000ff);
			// // 		ddDrawCircle(normal, center, 1.0f, 0.5f + bx::fsin(time*10.0f) );
			// // 	ddPop();

			// // 	ddSetSpin(time);
			// // 	ddDrawQuad(m_sprite, normal, center, 2.0f);
			// // }
			// // ddPop();

			// // ddPush();
			// // 	ddSetStipple(true, 1.0f, -time*0.1f);
			// // 	ddDrawCircle(Axis::Z, -8.0f, 0.0f, 0.0f, 1.25f, 2.0f);
			// // ddPop();

			// // ddPush();
			// // 	ddSetLod(UINT8_MAX);

			// // 	ddPush();
			// // 		ddSetSpin(time*0.3f);
			// // 		{
			// // 			float from[3] = { -11.0f, 4.0f,  0.0f };
			// // 			float to[3]   = { -13.0f, 6.0f,  1.0f };
			// // 			ddDrawCone(from, to, 1.0f );
			// // 		}

			// // 		{
			// // 			float from[3] = {  -9.0f, 2.0f, -1.0f };
			// // 			float to[3]   = { -11.0f, 4.0f,  0.0f };
			// // 			ddDrawCylinder(from, to, 0.5f );
			// // 		}
			// // 	ddPop();

			// // 	{
			// // 		float from[3] = {  0.0f, 7.0f, 0.0f };
			// // 		float to[3]   = { -6.0f, 7.0f, 0.0f };
			// // 		ddDrawCylinder(from, to, 0.5f, true);
			// // 	}
			// // ddPop();

			// // ddPush();

			// // 	float mtx[16];
			// // 	bx::mtxSRT(mtx
			// // 		, 1.0f, 1.0f, 1.0f
			// // 		, 0.0f, time, time*0.53f
			// // 		, -10.0f, 1.0f, 10.0f
			// // 		);

			// // 	Cylinder cylinder =
			// // 	{
			// // 		{ -10.0f, 1.0f, 10.0f },
			// // 		{ 0.0f, 0.0f, 0.0f },
			// // 		1.0f
			// // 	};

			// // 	float up[3] = { 0.0f, 4.0f, 0.0f };
			// // 	bx::vec3MulMtx(cylinder.m_end, up, mtx);
			// // 	ddDraw(cylinder);

			// // 	toAabb(aabb, cylinder);
			// // 	ddSetColor(0xff0000ff);
			// // 	ddDraw(aabb);

			// // ddPop();

		    // ddDrawOrb(-11.0f, 0.0f, 0.0f, 1.0f);

			// ddEnd();

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

	std::vector<pumpkin::PosColorVertex> m_scene_vertices;
	std::vector<uint16_t> m_scene_triangles;

	bgfx::DynamicVertexBufferHandle m_vbh;
	bgfx::DynamicIndexBufferHandle m_ibh;
//	bgfx::VertexBufferHandle m_vbh;
//	bgfx::IndexBufferHandle m_ibh;
	bgfx::ProgramHandle m_program;
};

ENTRY_IMPLEMENT_MAIN(FbxViewer);
