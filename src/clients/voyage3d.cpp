#include <graphics/GraphicsSystem.h>
#include <common/GameState.h>
#include <common/MainEntryPointHelper.h>

#include <OgreRenderWindow.h>
#include <OgreTimer.h>
#include <Threading/OgreThreads.h>


using namespace Demo;

namespace Demo
{
    class MyGraphicsSystem : public GraphicsSystem
    {
        // No resources. They're not needed and a potential point of failure.
        // This is a very simple project
        virtual void setupResources(void) {}
    public:
        MyGraphicsSystem( GameState *gameState ) :
            GraphicsSystem( gameState ) {}
    };
}

int mainApp() {

	GameState gameState;
	MyGraphicsSystem graphicsSystem( &gameState );

	graphicsSystem.initialize( "Tutorial 01: Initialization" );

	if( graphicsSystem.getQuit() )
	{
		graphicsSystem.deinitialize();
		return 0; //User cancelled config
	}

	Ogre::RenderWindow *renderWindow = graphicsSystem.getRenderWindow();

	graphicsSystem.createScene01();
	graphicsSystem.createScene02();

	Ogre::Timer timer;
	unsigned long startTime = timer.getMicroseconds();

	double timeSinceLast = 1.0 / 60.0;

	while( !graphicsSystem.getQuit() )
	{
		graphicsSystem.beginFrameParallel();
		graphicsSystem.update( static_cast<float>( timeSinceLast ) );
		graphicsSystem.finishFrameParallel();
		graphicsSystem.finishFrame();

		if( !renderWindow->isVisible() )
		{
			//Don't burn CPU cycles unnecessary when we're minimized.
			Ogre::Threads::Sleep( 500 );
		}

		unsigned long endTime = timer.getMicroseconds();
		timeSinceLast = (endTime - startTime) / 1000000.0;
		timeSinceLast = std::min( 1.0, timeSinceLast ); //Prevent from going haywire.
		startTime = endTime;
	}

	graphicsSystem.destroyScene();
	graphicsSystem.deinitialize();

	return 0;
}
