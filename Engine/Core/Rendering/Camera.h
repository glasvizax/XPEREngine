#pragma once
#include <glm/glm.hpp>

class Camera
{
	void rotatePitch(float degrees);
	void rotateYaw(float degrees);

	void move(glm::vec3 where);
	void setPosition(glm::vec3 new_position);

	void setZoom(float new_zoom);
	void setFovVertical(float fov);
	void setAspectRatio(float aspect_ratio);
	void setNearPlane(float near);
	void setFarPlane(float far);

	float getZoom();
	float getFovVertical();
	float getAspectRatio();
	float getNearPlane();
	float getFarPlane();

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

private:

	float m_yaw = -90.0f;
	float m_pitch = 0.0f;

	float m_aspect_ratio = 1.77f;
	float m_fov = 60.0f;

	float m_near = 0.1f;
	float m_far = 100.0f;

	glm::vec3 m_position{ 0.0f, 0.0f, 0.0f };

	glm::vec3 m_look_dir{ 0.0f, 0.0f, -1.0f };
	glm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
	glm::vec3 m_up{ 0.0f, 1.0f, 0.0f };

	glm::vec3 m_world_up{ 0.0f, 1.0f, 0.0f };

	float m_sensitivity = 0.1f;
	float m_zoom = 50.0f;

	void updateVectors();
};