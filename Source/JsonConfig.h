#include "Globals.h"
#include "Dependencies/Parson/parson.h"

#include "Dependencies/MathGeoLib/include/MathGeoLib.h" //To delete

#include <string> //To delete
using namespace std;
class Color;
class ArrayConfig;

class JsonConfig {
private:
	std::string path;
	JSON_Object* node = nullptr;
	JSON_Value* value = nullptr;

public:

	JsonConfig(const string path, JSON_Object* object, JSON_Value* value);
	~JsonConfig();


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


	ArrayConfig NewArray(const string name);
	ArrayConfig GetArray(const string name);
	ArrayConfig SetArray(const string name);


};


class ArrayConfig {
private:

	JSON_Array* jArray;


public:

	ArrayConfig(JSON_Array* jArray);
	~ArrayConfig();

	double GetNumber(const string name);
	void SetNumber(const string name, const double& number);

	bool GetBool(const string name);
	void SetBool(const string name, const bool boole);

	Color GetColor(const string name);
	void SetColor(const string name, const Color color);

	float3 GetFloat3(const string name);
	void SetFloat3(const string name, const float3 float3);

	Quat GetQuat(const string name);
	void SetQuat(const string name, const Quat quat);

	JsonConfig GetNode(string name, const uint index);
	JsonConfig SetNode(string name);

	uint Size();


};
