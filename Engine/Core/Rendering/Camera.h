#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	Camera(Camera&&) noexcept = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	void rotatePitch(float degrees);
	void rotateYaw(float degrees);

	void move(glm::vec3 where);
	void setPosition(glm::vec3 new_position);

	void setFovVertical(float fov);
	void setAspectRatio(float aspect_ratio);
	void setNearPlane(float _near);
	void setFarPlane(float _far);

	float getFovVertical();
	float getAspectRatio();
	float getNearPlane();
	float getFarPlane();

	glm::vec3 getLookVector();
	glm::vec3 getRightVector();
	glm::vec3 getUpVector();
		
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

	glm::mat4 view;
	glm::mat4 projection;

	bool m_view_dirty = true;
	bool m_proj_dirty = true;

	void updateVectors();
};