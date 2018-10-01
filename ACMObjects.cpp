#include "ACMObjects.h"

class Right
{
	public Right(std::string name, int id)
	{
		this.name = name;
		this.id = id;
	}
	std::string getName()
	{
		return name;
	}
	int getId()
	{
		return id;
	}	
};