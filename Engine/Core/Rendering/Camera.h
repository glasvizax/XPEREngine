#pragma once
#include <glm/glm.hpp>

#include "xm/xm.h"

class Camera
{
public:
	Camera() = default;
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;

	Camera(Camera&&) noexcept = delete;
	Camera& operator=(Camera&&) noexcept = delete;

	void rotateX(float degrees);
	void rotateY(float degrees);

	void move(xm::vec3 where);
	void setPosition(xm::vec3 new_position);

	void setFovVertical(float fov);
	void setAspectRatio(float aspect_ratio);
	void setNearPlane(float _near);
	void setFarPlane(float _far);

	float getFovVertical();
	float getAspectRatio();
	float getNearPlane();
	float getFarPlane();

	xm::vec3 getLookVector();
	xm::vec3 getRightVector();
	xm::vec3 getUpVector();

	xm::vec3 getPosition();

	xm::mat4			 getViewMatrix();
	xm::matrix<4, float> getProjectionMatrix();

	bool isViewDirty();

	bool isProjectionDirty();

private:
	float m_aspect_ratio = 1.77f;
	float m_fov = 60.0f;

	float m_near = 0.1f;
	float m_far = 2200.0f;

	xm::vec3 m_position{ 0.0f, 0.0f, 0.0f };

	xm::vec3 m_look_dir{ 0.0f, 0.0f, -1.0f };
	xm::vec3 m_right{ 1.0f, 0.0f, 0.0f };
	xm::vec3 m_up{ 0.0f, 1.0f, 0.0f };

	xm::vec3 m_world_up{ 0.0f, 1.0f, 0.0f };

	xm::mat4 m_view;
	xm::mat4 m_projection;

	xm::vec3 m_rotation{ 0.0f, 0.0f, 0.0f };
	xm::quat m_quat_rotatiom;

	bool m_view_dirty = true;
	bool m_proj_dirty = true;

	void updateVectors();
};