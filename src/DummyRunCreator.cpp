#include "DummyRunCreator.h"
#include "KRDummyRun.h"
#include "THDummyRun.h"

DummyRunCreator* DummyRunCreator::_self = NULL;

DummyRunCreator::DummyRunCreator(void)
{
}

DummyRunCreator::~DummyRunCreator(void)
{
}

DummyRunCreator*  DummyRunCreator::GetInstance()
{
	if(NULL == _self)
		_self = new DummyRunCreator;

	return _self;
}

// 생성 매서드
DummyRun* DummyRunCreator::CreateDummyRun(const DUMMY_TYPE type)
{
	DummyRun* dmr = NULL;

	switch(type)
	{
	case DUMMYTYPE_KR:
		dmr = new KRDummyRun;
		break;
	case DUMMYTYPE_TH:
		dmr = new THDummyRun;
		break;
	default:
		dmr = NULL;
		break;
	}

	return dmr;
}