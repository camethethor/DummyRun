#include <tchar.h>
#include "Config.h"
#include "NetworkHandler.h"
#include "DummyRunManager.h"

int _tmain( int argc, _TCHAR* argv[] )
{
	if( NetworkHandler::GetInstance()->Initialize() == false )
	{
		return false;
	}

	if( DummyRunManager::GetInstance()->Initialize() == false )
	{
		return false;
	}

	for(int i = 0; i <= DUMMY_TRY_COUNT; i++ )
	{
		DummyRunManager::GetInstance()->Start();
		WaitForSingleObject( INVALID_HANDLE_VALUE, RECONNECT_INTERVAL );
	}
}

