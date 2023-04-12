#include "DummyRunManager.h"
#include "DummyRunCreator.h"
#include "Config.h"
#include "Locker.h"

DummyRunManager* DummyRunManager::_self = NULL;

DummyRunManager::DummyRunManager(void) :_active_count(0) //, _inactive_count(0)
{
	_dummy_container.Clear();
}

DummyRunManager::~DummyRunManager(void)
{
    DummyRun*	dmy_run = NULL;
	Locker	lock_obj(_dummy_container);

    Map<unsigned long, DummyRun*>::MAP_POSITION pos = _dummy_container.HeadPosition();

    while(false == _dummy_container.IsEnd(pos))
    {		
        dmy_run = pos->second;
        dmy_run->Destroy();

        ++pos;
        delete dmy_run;
    }
	ReleaseInstance();
}

DummyRunManager* DummyRunManager::GetInstance()
{
	if(_self == NULL)
		_self = new DummyRunManager();

	return _self;
}

void DummyRunManager::ReleaseInstance()
{  
	if(_self != NULL)
    {
        delete _self;
        _self = NULL;
    }
}

bool DummyRunManager::Initialize()
{
	DummyRun* dmr = NULL;

	for(int index = 0; index < SESSION_COUNT; ++index)
	{
		dmr = DummyRunCreator::GetInstance()->CreateDummyRun(DUMMYTYPE_KR);

		if(NULL != dmr)
		{
			bool ret = dmr->Initialize(index);

			if(ret)
			{
				_dummy_container.Insert(index, dmr);
				return true;
			}
		}
	}
	return false;
}

//bool DummyRunManager::RemoveSession(Session* session, int type)
//{
//	if(session == NULL)
//		return false;
//
//	Dummy* dmy = NULL;
//	Locker sync(&_dummy_container);
//	Map<unsigned short, Dummy*>::MAP_POSITION pos = _dummy_container.HeadPosition();
//
//	while(false == _dummy_container.IsEnd(pos))
//	{
//		dmy = pos->second;
//		dmy->Destroy();
//		
//		++pos;
//		delete dmy;
//	}
//
//	//List< Object* >::LIST_POSITION pos = _session_list.Find(session);
//
//	//if(_session_list.IsEnd(pos) != true)
//	//{
//	//	_session_list.RemoveItem(pos);
//	//	session->OnDestroy();
//	//}
//	//else
//	//	return false;
//
//	return true;
//}

//bool DummyRunManager::Run()
//{
//	List<Object*>::LIST_POSITION pos = _session_list.HeadPosition();
//
//	while(!_session_list.IsEnd(pos))
//	{
//		static_cast<Session*>(*pos)->OnCreate();
//		++pos;
//	}
//
//	return true;
//}

bool DummyRunManager::Start()
{
	Map<unsigned long, DummyRun*>::MAP_POSITION pos = _dummy_container.HeadPosition();

	while(false == _dummy_container.IsEnd(pos))
	{
		if(false == pos->second->GetAlive())
		{
			pos->second->Run();
			//Sleep(100);
		}
		++pos;
	}
	
	return true;
}

//bool DummyRunManager::SessionDestroy(unsigned long session_id)
//{
//    Map<unsigned long, Dummy*>::MAP_POSITION pos = _dummy_container.Find(session_id);
//    if(!_dummy_container.IsEnd(pos))
//    {
//        Dummy* dummy = pos->second;
//        dummy->Destroy();
//        return true;
//    }    
//
//    return false;
//}


//bool DummyRunManager::ReStartSession(unsigned long session_id)
//{
//    Map<unsigned long, Dummy*>::MAP_POSITION pos = _dummy_container.Find(session_id);
//    if(!_dummy_container.IsEnd(pos))
//    {
//        Dummy* dummy = pos->second;
//        dummy->LoginSessionRun();
//        return true;
//    }    
//
//    return false;
//}



