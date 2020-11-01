#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "ComponentTransform.h"
#include "GameObject.h"
#include "ModuleSceneIntro.h"

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
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

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
	ComponentTransform* selected_obj = (ComponentTransform*)App->scene_intro->selected_object->GetComponent(ComponentType::Transform);
	
	vec3 ObjPosition;
	
	ObjPosition.x = selected_obj->GetPosition().x;
	ObjPosition.y = selected_obj->GetPosition().y;
	ObjPosition.z = selected_obj->GetPosition().z;

	vec3 ObjScale;

	ObjScale.x = selected_obj->GetScale().x;
	ObjScale.y = selected_obj->GetScale().y;
	ObjScale.z = selected_obj->GetScale().z;

	Position = (ObjPosition + normalize(Position - ObjPosition)) * 8;
	//Position.y = ;
	//Position.z = ;

	LookAt(ObjPosition);
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) FocusObject();

	vec3 newPos(0,0,0);
	float speed = 8.0f * dt;
	if(App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 16.0f * dt;

	if(App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) newPos.y += speed;
	if(App->input->GetKey(SDL_SCANCODE_X) == KEY_REPEAT) newPos.y -= speed;

	if(App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if(App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if(App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if(App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	if (App->input->GetMouseZMotion() != 0)
	{
		if (App->input->GetMouseZMotion() < 0) {
			newPos += Z * speed * 4;
		}
		else if (App->input->GetMouseZMotion() > 0) {
			newPos -= Z * speed * 4;
		}
	}

	Position += newPos;
	Reference += newPos;

	

	// Mouse motion ----------------

	if(App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if(dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if(dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if(Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);
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
	return &ViewMatrix;
}

mat4x4 ModuleCamera3D::GetViewMatrix()
{
	CalculateViewMatrix();
	return ViewMatrix;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}
