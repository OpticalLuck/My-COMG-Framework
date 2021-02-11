#include "CameraVer2.h"
#include "Mtx44.h"
#include "Application.h"

CameraVer2::CameraVer2():
	target(Vector3(0,0,0)),
	mode(THIRD_PERSON),
	sensitivity(0.05f),
	speed(5.f),
	distance(8.f),
	IsKeyPressed(false),
	IsJump(false),
	IsGround(true),
	Controls(true)
{
}

CameraVer2::~CameraVer2()
{
}

void CameraVer2::Init(Vector3 position, const Vector3& view, const Vector3& up)
{
	this->position = position;
	this->view = view.Normalized();
	this->up = up.Normalized();
}

void CameraVer2::Update(float x_offset, float y_offset)
{
	if (Controls)
	{
		Mtx44 RotateYaw;
		RotateYaw.SetToRotation(-x_offset * sensitivity, 0, 1, 0);
		view = RotateYaw * view;

		float AvailableUP = Math::RadianToDegree(acosf(view.Dot(up))) - 0.4f;
		float AvailableDOWN = Math::RadianToDegree(acosf(view.Dot(-up))) - 0.4f;
	
		float Angle = y_offset * sensitivity;
		Angle = Math::Clamp(Angle, -AvailableDOWN, AvailableUP);

		Mtx44 RotatePitch;
		Vector3 right = GetRight();
		RotatePitch.SetToRotation(Angle, right.x, right.y, right.z);
		view = RotatePitch * view;

		if (mode == THIRD_PERSON)
		{
			position = target + distance * -view;
		}

	}
}

void CameraVer2::Updatemovement(double dt)
{
	if (Controls)
	{
		Vector3 direction = Vector3(view.x, 0, view.z).Normalized();
		if (Application::IsKeyPressed('W'))
		{
			if(mode == FREE_VIEW)
			{
				position += view * dt * speed;
				target += view * dt * speed;
			}

			position.x += direction.x * dt * speed;
			position.z += direction.z * dt * speed;

			target.x += direction.x * dt * speed;
			target.z += direction.z * dt * speed;
		}
		if (Application::IsKeyPressed('S'))
		{
			if (mode == FREE_VIEW)
			{
				position -= view * dt * speed;
				target -= view * dt * speed;
			}
			else
			{
				position.x -= direction.x * dt * speed;
				position.z -= direction.z * dt * speed;

				target.x -= direction.x * dt * speed;
				target.z -= direction.z * dt * speed;
			}
		}
		if (Application::IsKeyPressed('A'))
		{
			position -= GetRight() * dt * speed;
			target -= GetRight() * dt * speed;
		}
		if (Application::IsKeyPressed('D'))
		{
			position += GetRight() * dt * speed;
			target += GetRight() * dt * speed;
		}
	
		if (Application::IsKeyPressed(VK_SPACE) && !IsKeyPressed)
		{
			IsKeyPressed = true;
			if (IsGround)
			{
				IsJump = true;
			}
		}
		else if (!Application::IsKeyPressed(VK_SPACE) && IsKeyPressed)
		{
			IsKeyPressed = false;
		}
		if (Application::IsKeyPressed(VK_LSHIFT))
		{
			speed = 10.0f;
		}
		else
		{
			speed = 5.0f;
		}


		if(mode == THIRD_PERSON)
		{
			Jump(dt);

			target.x = Math::Clamp(target.x, -30.f, 30.f);
			target.y = Math::Clamp(target.y, 3.f, 5.f);
			target.z = Math::Clamp(target.z, -30.f, 30.f);
		}
		else if (mode == FIRST_PERSON)
		{
			Jump(dt);

			position.x = Math::Clamp(position.x, -30.f, 30.f);
			position.y = Math::Clamp(position.y, 3.f, 5.f);
			position.z = Math::Clamp(position.z, -30.f, 30.f);
		}
	}
}

void CameraVer2::SetTarget(Vector3 target)
{
	this->target = target;
}

void CameraVer2::SetPosition(Vector3 position)
{
	this->position = position;
}

void CameraVer2::SetView(Vector3 view)
{
	this->view = view.Normalized();
}

void CameraVer2::Jump(double dt)
{
	float JumpSpeed = 5.0f;
	if (mode == THIRD_PERSON)
	{
		if (IsJump)
		{
			IsGround = false;
			if (target.y < 5.f)
			{
				
				target.y += JumpSpeed * dt;
			}
			else
			{
				IsJump = false;
			}
		}
		else
		{
			if (target.y > 3.f)
			{
				target.y -= JumpSpeed * dt;
			}
			else
			{
				IsGround = true;
			}
		}
	}
	else
	{
		if (IsJump)
		{
			IsGround = false;
			if (position.y < 5.f)
			{

				position.y += JumpSpeed * dt;
			}
			else
			{
				IsJump = false;
			}
		}
		else
		{
			if (position.y > 3.f)
			{
				position.y -= JumpSpeed * dt;
			}
			else
			{
				IsGround = true;
			}
		}
	}
}

void CameraVer2::ToggleMode(CAMERA_MODE mode)
{
	prev_mode = this->mode;
	this->mode = mode;
}

void CameraVer2::ToggleAnimation(float distance)
{
	float WatchingDist = 8.f;
	if (mode != ANIMATION)
	{

		if (mode == THIRD_PERSON)
		{
			//position = target;
		}
		Controls = false;
		prev_mode = mode;
		mode = ANIMATION;

		position += WatchingDist * GetRight() + (distance * 0.5f) * view.Normalized();
		view = (GetRight() * -1).Normalized();
	}
	else
	{
		view = GetRight();
		Controls = true;
		mode = prev_mode;
	}
}

void CameraVer2::ToggleControls(bool Controls)
{
	this->Controls= Controls;
}

Vector3 CameraVer2::GetRight() const
{
	return view.Cross(up).Normalized();
}

float CameraVer2::GetYaw() const
{
	return Math::RadianToDegree(atan2(view.z,view.x));
}

float CameraVer2::GetPitch() const
{
	return Math::RadianToDegree(acosf(view.Dot(up))) - 90;
}

const Vector3 CameraVer2::GetObjPos()
{
	if (mode == THIRD_PERSON)
	{
		return position + view * distance;
	}
	else
	{
		return position;
	}
}



const Vector3& CameraVer2::GetPosition()
{
	// TODO: insert return statement here
	return position;
}

const Vector3& CameraVer2::GetView()
{
	// TODO: insert return statement here
	return view;
}

const Vector3& CameraVer2::GetUp()
{
	// TODO: insert return statement here
	return up;
}

const CameraVer2::CAMERA_MODE& CameraVer2::GetPrevMode()
{
	// TODO: insert return statement here
	return prev_mode;
}

const CameraVer2::CAMERA_MODE& CameraVer2::GetMode()
{
	// TODO: insert return statement here
	return mode;
}

std::string CameraVer2::GetStrMode() const
{
	switch (mode)
	{
	case FIRST_PERSON:
		return "First Person";
	case THIRD_PERSON:
		return "Third Person";
	case FREE_VIEW:
		return "Free View";
	case ANIMATION:
		return "Animation";
	};
}

