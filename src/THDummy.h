#include "Dummy.h"

class THDummy : public Dummy
{
public:
	THDummy(const unsigned int id);
	virtual ~THDummy(void);
};

THDummy::THDummy(const unsigned int id) : Dummy(id) 
{
}

THDummy::~THDummy(void) 
{
}