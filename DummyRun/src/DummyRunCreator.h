#pragma once

enum DUMMY_TYPE
{
	DUMMYTYPE_NONE,
	DUMMYTYPE_KR,
	DUMMYTYPE_TH,
};

class DummyRun;
class DummyRunCreator
{
public:
	static DummyRunCreator* GetInstance();
	DummyRun* CreateDummyRun( const DUMMY_TYPE type );

private:
	DummyRunCreator(void);
	~DummyRunCreator(void);

	static DummyRunCreator* _self;
};
