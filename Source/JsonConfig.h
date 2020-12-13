#ifndef __JSONCONFIG_H__
#define __JSONCONFIG_H__

#include "Dependencies/Parson/parson.h"
//#include "Dependencies/MathGeoLib/include/MathGeoLib.h" //To delete
#include "Dependencies/MathGeoLib/include/Math/float3.h"

#include "Globals.h"



struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t  JSON_Array;

using namespace std;
class Color;
class ArrayConfig;

class JsonConfig {
private:
	std::string path;
	JSON_Object* node = nullptr;
	JSON_Value* value = nullptr;

public:

	JsonConfig();
	JsonConfig(const char* buffer);
	JsonConfig(const string path, JSON_Object* object, JSON_Value* value);
	JsonConfig(JSON_Object* jsonObject);
	~JsonConfig();

	uint SerializeConfig(char** buffer);

	double GetNumber(const string name);
	void SetNumber(const string name, const double number);

	string GetString(const string name) const;
	void SetString(string name, string destination);

	bool GetBool(const string name);
	void SetBool(const string name, const bool boolean);

	Color GetColor(const string name);
	void SetColor(const string name, const Color color);

	float3 GetFloat3(const string name);
	void SetFloat3(const string name, const float3 float3);

	Quat GetQuat(const string name);
	void SetQuat(const string name, const Quat quat);

	ArrayConfig GetArray(const string name) const;
	ArrayConfig SetArray(const string name);


};

class ArrayConfig {
private:

	JSON_Array* jArray;
	uint index = 0;

public:

	ArrayConfig();
	ArrayConfig(JSON_Array* jArray);
	~ArrayConfig();

	//double GetNumber(const string name);
	//void SetNumber(const string name, const double& number);

	//bool GetBool(const string name);
	//void SetBool(const string name, const bool boole);

	//Color GetColor(const string name);
	//void SetColor(const string name, const Color color);

	//float3 GetFloat3(const string name);
	//void SetFloat3(const string name, const float3 float3);

	//Quat GetQuat(const string name);
	//void SetQuat(const string name, const Quat quat);

	//JsonConfig GetNode(string name, const uint index);
	//JsonConfig SetNode(string name);
	JsonConfig AddNode();
	JsonConfig GetNode(uint index) const;
	uint GetSize() const;

	//uint Size();


};
#endif //__JSONCONFIG_H__