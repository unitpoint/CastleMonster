#include "oxygine-framework.h"
#include <ox-binder.h>

using namespace oxygine;

void example_preinit()
{
	ObjectScript::Oxygine::init();
}

#ifdef _WIN32
#include <core\STDFileSystem.h>
file::STDFileSystem extfs(true);
file::STDFileSystem extfs2(true);
#endif

void example_postinit()
{
#ifdef _WIN32
	extfs.setPath(file::fs().getFullPath("../data").c_str());
	file::mount(&extfs);
	
	extfs2.setPath(file::fs().getFullPath("data").c_str());
	file::mount(&extfs2);
#endif
	ObjectScript::Oxygine::postInit();
}

void example_init()
{
	ObjectScript::Oxygine::run();
}

void example_update()
{
#if defined WIN32 && !defined OS_DEBUG
	sleep(10);
#endif
}

void example_destroy()
{
	ObjectScript::Oxygine::shutdown();
}