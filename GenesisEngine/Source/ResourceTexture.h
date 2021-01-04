#pragma once
#include "Globals.h"
#include "Resource.h"

typedef unsigned char GLubyte;
class GnJSONObj;

class ResourceTexture : public Resource {
public:
	ResourceTexture(uint UID);
	~ResourceTexture();

	void GenerateBuffers();
	void BindTexture();

	void FillData(GLubyte* data, uint id, int width, int height);
	uint SaveMeta(GnJSONObj& base_object, uint last_modification) override;
	void Load(GnJSONObj& base_object) override;

	uint GetID();
	int GetWidth();
	int GetHeight();
	GLubyte* GetData();
	uint GetGpuID();

public:
	enum
	{
		DIFFUSE_MAP,
		NORMAL_MAP,
		UNKNOWN_MAP
	}texture_type = UNKNOWN_MAP;

private:
	uint _id;
	int _width;
	int _height;
	GLubyte* _data;
	uint _gpu_ID;
};
