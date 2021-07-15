#include "field.h"
#include <assert.h>
Field::Field(SceneBase* scene) : GameObject(scene)
{
	hModel = MV1LoadModel("data\\model\\stage\\MMDˆê•“¹‰ïêVer3.pmx");
	assert(hModel > 0);
	hSkyModel = MV1LoadModel("data\\model\\stage\\stage00_sky.mv1");
	assert(hSkyModel > 0);
}

Field::~Field()
{
	if(hModel > 0)
		MV1DeleteModel(hModel);
}

void Field::Update()
{
}

void Field::Draw()
{
	MV1DrawModel(hModel);
	MV1DrawModel(hSkyModel);
}

bool Field::CollisionLine(VECTOR* hit, VECTOR form, VECTOR to)
{
	MV1_COLL_RESULT_POLY col = MV1CollCheck_Line(hModel, -1, form, to);
	if (col.HitFlag == 0)
		return false;
	*hit = col.HitPosition;
	return true;
}
