#ifndef MODEL_H_
#define MODEL_H_

#include "WorldOptionsData.h"

class Model
{
public:
	Model();
	virtual ~Model();
	
	
private:
	
// world options
	WorldOptionsData* worldOptionsData;
	
};

#endif /*MODEL_H_*/