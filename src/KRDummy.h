#include "Dummy.h"

class KRDummy : public Dummy
{
public:
	KRDummy(const unsigned int id);
	virtual ~KRDummy(void);
};

KRDummy::KRDummy(const unsigned int id) : Dummy(id) 
{
}

KRDummy::~KRDummy(void) 
{
}