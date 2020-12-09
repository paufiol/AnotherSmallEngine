#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ComponentCamera.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
}

ModuleCamera3D::~ModuleCamera3D()
{
	delete currentCamera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	editorCamera = currentCamera = new ComponentCamera(nullptr);

	gameCamera = nullptr;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::FocusObject() 
{
	vec3 ObjPosition;
	
	ObjPosition.x = App->scene->selected_object->transform->GetPosition().x;
	ObjPosition.y = App->scene->selected_object->transform->GetPosition().y;
	ObjPosition.z = App->scene->selected_object->transform->GetPosition().z;

	vec3 ObjScale;

	ObjScale.x = App->scene->selected_object->transform->GetScale().x;
	ObjScale.y = App->scene->selected_object->transform->GetScale().y;
	ObjScale.z = App->scene->selected_object->transform->GetScale().z;

	Position = (ObjPosition + normalize(Position - ObjPosition));

	LookAt(ObjPosition);
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	if (App->scene->selected_object != nullptr)
	{
		reference = App->scene->selected_object->transform->GetPosition();
	}
	else reference = float3(0.0f, 0.0f, 0.0f);	//TODO: Set a center point at center of far Plane?

	int x = App->input->GetMouseXMotion();
	int y = App->input->GetMouseYMotion();

	if (App->input->GetKey(SDL_SCANCODE_LALT) && (x != 0 || y != 0))
	{

		float dx = (float)x * dt;
		float dy = (float)y * dt;


		//TODO: Add Magic Numbers as Control Settings via IMGUI
		//Pan
		if(App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
		{
			currentCamera->Pan(dx*50.0f, dy*50.0f);
		}

		//Rotate
		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT))
		{
			currentCamera->LookAt(-dx*.3f, -dy*.3f); 
		}
	}

	if (App->input->GetMouseZMotion() != 0)
	{
		float dz = (float)App->input->GetMouseZMotion() * dt;
		currentCamera->Zoom(dz*30.0f);
	}	
	
	if(!App->editor->GUIhovered)
	{
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			//float2 mousePos = ();
			//currentCamera->OnClick();

			//RAYCAST BULLSHIT HERE
		}
	}



	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetRawViewMatrix()
{
	CalculateViewMatrix();


	return 	currentCamera->GetViewMatrix();
}

mat4x4 ModuleCamera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix() //View Matrix from 
{
	
	
	
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
