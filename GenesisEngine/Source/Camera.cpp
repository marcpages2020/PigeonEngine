#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ImGui/imgui.h"

Camera::Camera() : Component(nullptr), _aspectRatio(AspectRatio::AR_16_9) {
	type = ComponentType::CAMERA;

	_frustum.type = FrustumType::PerspectiveFrustum;

	_frustum.pos = float3(0.0f, 0.0f, 0.0f);
	_frustum.up = float3(0.0f, 1.0f, 0.0f);
	_frustum.front = float3(0.0f, 0.0f, 1.0f);

	//_frustum.horizontalFov = 60.0f * DEGTORAD;
	_frustum.verticalFov = 30.0f * DEGTORAD;
	AdjustAspecRatio();

	_frustum.nearPlaneDistance = 0.3f;
	_frustum.farPlaneDistance = 20.0f;
}

Camera::Camera(GameObject* gameObject) : Component(gameObject), _aspectRatio(AspectRatio::AR_16_9)
{
	type = ComponentType::CAMERA;
	_gameObject = gameObject;

	_frustum.type = FrustumType::PerspectiveFrustum;

	_frustum.pos = float3(0.0f, 0.0f, 0.0f);
	_frustum.up = float3(0.0f, 1.0f, 0.0f);
	_frustum.front = float3(0.0f, 0.0f, 1.0f);

	//_frustum.horizontalFov = 60.0f * DEGTORAD;
	_frustum.verticalFov = 30.0f * DEGTORAD;
	AdjustAspecRatio();

	_frustum.nearPlaneDistance = 0.3f;
	_frustum.farPlaneDistance = 20.0f;
}

Camera::~Camera(){}

void Camera::Update()
{
	_frustum.pos = _gameObject->GetTransform()->GetPosition();
	//_frustum.Transform(_gameObject->GetTransform()->GetLocalTransform());
	
	_frustum.up = _gameObject->GetTransform()->GetGlobalTransform().WorldY();
	_frustum.front = _gameObject->GetTransform()->GetGlobalTransform().WorldZ();

	float3 corner_points[8];
	_frustum.GetCornerPoints(corner_points);
	App->renderer3D->DrawAABB(corner_points);
}

void Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		float horizontalFOV = _frustum.horizontalFov * RADTODEG;
		if (ImGui::DragFloat("Horizontal FOV", &horizontalFOV, 0.02f, 0.0f, 130.0f)) {
			_frustum.horizontalFov = horizontalFOV * DEGTORAD;
			AdjustAspecRatio();
		}

		float verticalFOV = _frustum.verticalFov * RADTODEG;
		if (ImGui::DragFloat("Vertical FOV", &verticalFOV, 0.02f, 0.0f, 60.0f)) {
			_frustum.verticalFov = verticalFOV * DEGTORAD;
			AdjustAspecRatio();
		}

		ImGui::Spacing();
	}
}

void Camera::AdjustAspecRatio()
{
	switch (_aspectRatio)
	{
	case AR_16_9:
		_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (16.0f / 9.0f));
		//_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (9 / 16));
		break;
	case AR_16_10:
		_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (16.0f / 10.0f));
		//_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (10 / 16));
		break;
	case AR_4_3:
		_frustum.horizontalFov = 2.0f * std::atan(std::tan(_frustum.verticalFov * 0.5f) * (4.0f / 3.0f));
		//_frustum.verticalFov = 2 * atan(tan(_frustum.horizontalFov * 0.5f) * (3 / 4));
		break;
	default:
		break;
	}
}

void Camera::SetPosition(float3 position)
{
	_frustum.pos = position;
}

void Camera::SetReference(float3 reference)
{
	_reference = reference;
}

void Camera::Look(float3 spot)
{
	float3 difference = spot - _frustum.pos;

	float3x3 matrix = float3x3::LookAt(_frustum.front, difference.Normalized(), _frustum.up, float3::unitY);

	_frustum.front = matrix.MulDir(_frustum.front).Normalized();
	_frustum.up = matrix.MulDir(_frustum.up).Normalized();
}

Frustum Camera::GetFrustum()
{
	return _frustum;
}

float* Camera::GetViewMatrix()
{
	float4x4 viewMatrix;

	viewMatrix = _frustum.ViewMatrix();
	viewMatrix.Transpose();

	return (float*)viewMatrix.v;
}
