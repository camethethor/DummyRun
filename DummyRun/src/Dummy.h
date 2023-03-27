#pragma once
#include <string>

class Dummy
{
public:
	Dummy(const unsigned int id);
	virtual ~Dummy(void);

	void	Initialize();
	void	SetCodeName(const std::string& src)	{ _code_name =  src; }

	unsigned int			GetID()			{ return _id; }
	bool					IsRecruit()		{ return _new_user; }
	const std::string&		GetCodeName()	{ return _code_name; }

private:
	unsigned int	_id;
	bool			_new_user;
	std::string		_code_name;
};