#include "Camera.h"

Camera::Camera()
{
	mouse_sensitivity = 0.05f;
	move_sensitivity = 2.5f;
	c_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	c_Front = glm::vec3(0.0f, 0.0f, 1.0f);
	c_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	fov = 45.0f;
}

Camera::Camera(float sens_Move, float sens_Look, int width, int height) {
	mouse_sensitivity = sens_Look;
	move_sensitivity = sens_Move;
	c_Pos = glm::vec3(0.0f, 0.0f, 0.0f);
	c_Front = glm::vec3(0.0f, 0.0f, 1.0f);
	c_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
	fov = 45.0f;
}

void Camera::setLookSensitivity(float sensitivity) {
	mouse_sensitivity = sensitivity;
}

void Camera::setMoveSensitivity(float sensitivity) {
	move_sensitivity = sensitivity;
}

float Camera::getLookSensitivity() {
	return mouse_sensitivity;
}
float Camera::getMoveSensitivity() {
	return move_sensitivity;
}

void Camera::setNextViewMatrix(glm::mat4 _matrix) {
	_nextViewMatrix = _matrix;
}

void Camera::setViewMatrix(glm::mat4 _matrix) {
	_viewMatrix = _matrix;
}

void Camera::lerpViewMatrix() {
	_nextViewMatrix /= 60.0f;
	_viewMatrix += _nextViewMatrix;
}

void Camera::lerpTransform() {
	c_Pos += newPos / 60.0f;
}


void Camera::lerpMouse(float _yaw, float _pitch) {
	if (yaw == r_preX) {
		r_newX = 0;
	}
	if (pitch == r_preY) {
		r_newY = 0;
	}

	yaw = glm::mod(yaw + (r_newX / 60.0f), 360.0f);  //Adds a 60th of the change - should be delta time of change.
	pitch += (r_newY) / 60.0f;
	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }

	if (pitch + _pitch > 89.0f) { _pitch = 89-pitch; }
	if (pitch + _pitch < -89.0f) { _pitch = -89-pitch; }
	glm::vec3 t_front;
	t_front.x = cos(glm::radians(pitch + _pitch)) * cos(glm::radians(glm::mod(yaw + _yaw, 360.0f)));
	t_front.y = sin(glm::radians(pitch + _pitch));
	t_front.z = cos(glm::radians(pitch + _pitch)) * sin(glm::radians(glm::mod(yaw + _yaw, 360.0f)));
	c_Front = glm::normalize(t_front);
}

void Camera::lerpMouse() {
	if (yaw == r_preX) {
		r_newX = 0;
	}
	if (pitch == r_preY) {
		r_newY = 0;
	}

	yaw = glm::mod((yaw + r_newX) / 60.0f, 360.0f);
	pitch += r_newY / 60.0f;
	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }
	glm::vec3 t_front;
	t_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	t_front.y = sin(glm::radians(pitch));
	t_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	c_Front = glm::normalize(t_front);
}

float Camera::getPitch() {
	return pitch;
}
float Camera::getYaw() {
	return yaw;
}
float Camera::getRoll() {
	//Like leaning round a corner
	return roll;
}

void Camera::setPitch(float _pitch) {
	r_newY = 0;
	pitch = _pitch;
}
void Camera::setYaw(float _yaw) {
	r_newX = 0;
	yaw = _yaw;
}
void Camera::setRoll(float _roll) {
	//r_newZ = 0;
	roll = _roll;
}


void Camera::calcViewMatrix(glm::vec3 _pos) {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(c_Pos + _pos, //Camera position
		c_Pos + _pos + c_Front, //Target
		c_Up); //Up vector
	_viewMatrix = view;
}

void Camera::calcViewMatrix() {
	glm::mat4 view = glm::mat4(1.0f);
	view = glm::lookAt(c_Pos, //Camera position
		c_Pos + c_Front, //Target
		c_Up); //Up vector
	_viewMatrix = view;
}

glm::mat4 Camera::viewMatrix() {
	return _viewMatrix;
}

void Camera::setPos(glm::vec3 _newPOS) {
	c_Pos = _newPOS;
}

void Camera::setPos(float x, float y, float z) {
	c_Pos = glm::vec3(x, y, z);
}

void Camera::setFOV(float _fov) {
	fov = _fov;
}

float Camera::getFOV() {
	return fov;
}

glm::vec2 Camera::getRot() {
	return glm::vec2(yaw, pitch);
}

glm::vec3 Camera::getPos() {
	return c_Pos;
}
glm::vec3 Camera::getFront() {
	return c_Front;
}
glm::vec3 Camera::getUp() {
	return c_Up;
}
glm::vec3 Camera::getRight() {
	return c_Right;
}

void Camera::KEY_INPUT(int KEY, float deltaTime) {
	float c_Speed = 6.5f * deltaTime;

	if (KEY == 400) {
		if (modeSwap > 10) {
			fpsMode = !fpsMode;
			gravity = !gravity;
			modeSwap = 0;
		}

	}

	if (KEY == 5) {
		//Up
		
		if (fpsMode) {
			if (jumpTime == 0) {
				//if (currentChunk.getBlock(c_Pos.x, c_Pos.y - 2 + yBound, c_Pos.z) != 0) {
				//	jumpTime += 1;
				//}
			}
		} else {
			c_Pos += c_Speed * glm::vec3(0.0f, 1.0f, 0.0f);
		}

	}
	if (KEY == 6) {
		//Down
		c_Pos += c_Speed * glm::vec3(0.0f, -1.0f, 0.0f);
	}
	if (KEY == 1) {
		//Forward.
		if (fpsMode) {
			glm::vec3 newPos = c_Pos + (c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z));
			c_Pos += c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
			//if (currentChunk.getBlock(floor(getBlock() + newPos).x, floor(getBlock() + newPos).y, floor(getBlock() + newPos).z) == 0) {
				//c_Pos += c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
			//}
			//else {
				//c_Pos += glm::vec3(0.0f, 5.0f, 0.0f);
			//}
		}
		else {
			c_Pos += c_Speed * c_Front;
		}
	}
	else if (KEY == 2) {
		//Backward
		if (fpsMode) {
			c_Pos -= c_Speed * glm::vec3(c_Front.x, 0.0f, c_Front.z);
		}
		else {
			c_Pos -= c_Speed * c_Front;
		}
		
	}
	if (KEY == 3) {
		//Left
		c_Pos -= c_Speed * glm::normalize(glm::cross(c_Front, c_Up));
	}
	else if (KEY == 4) {
		//Right
		c_Pos += c_Speed * glm::normalize(glm::cross(c_Front, c_Up));
	}
}


void Camera::MOUSE_MOVE(int x_offset, int y_offset) {
	float sensitivity = 0.05f;
	x_offset *= sensitivity;
	y_offset *= sensitivity;

	yaw = glm::mod(yaw + x_offset, 360.0f);
	pitch += y_offset;
	if (pitch > 89.0f) { pitch = 89.0f; }
	if (pitch < -89.0f) { pitch = -89.0f; }
	glm::vec3 t_front;
	t_front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	t_front.y = sin(glm::radians(pitch));
	t_front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	c_Front = glm::normalize(t_front);
}