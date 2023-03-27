#include "Dummy.h"
#include "Config.h"

Dummy::Dummy(const unsigned int id)
:_id(id), _new_user(false)
{
}

Dummy::~Dummy(void)
{

}

void Dummy::Initialize()
{
	_new_user = false;
	_code_name.clear();
	
	char code_name[MAX_NICKNAME_LENGTH]=""; 
	sprintf_s( code_name, "%d", _id);
	_code_name = code_name;
}

