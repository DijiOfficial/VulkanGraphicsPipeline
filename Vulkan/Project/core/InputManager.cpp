#include "InputManager.h"
#include "vulkanbase/VulkanBase.h"

void InputManager::Init(GLFWwindow* window)
{
	glfwSetWindowUserPointer(window, this);
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			InputManager* vBase = static_cast<InputManager*>(pUser);
			vBase->KeyEvent(key, scancode, action, mods);
		});
	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double xpos, double ypos)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			InputManager* vBase = static_cast<InputManager*>(pUser);
			vBase->MouseMove(window, xpos, ypos);
		});
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			InputManager* vBase = static_cast<InputManager*>(pUser);
			vBase->MouseEvent(window, button, action, mods);
		});
	glfwSetCursorEnterCallback(window, [](GLFWwindow* window, int entered)
		{
			void* pUser = glfwGetWindowUserPointer(window);
			InputManager* vBase = static_cast<InputManager*>(pUser);
			vBase->CursorEnter(entered);
		});
}

bool InputManager::IsMouseIdle() const
{
	auto now = std::chrono::steady_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_LastMovementTime);

	return duration > std::chrono::milliseconds(20);
}

void InputManager::KeyEvent(int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS)
		m_PressedKeys.insert(key);
	else if (action == GLFW_RELEASE)
		m_PressedKeys.erase(key);
}

void InputManager::MouseMove(GLFWwindow* window, double xpos, double ypos)
{
	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);
	if (state == GLFW_PRESS)
	{
		const float dx = m_DragStart.x - static_cast<float>(xpos);
		const float dy = m_DragStart.y - static_cast<float>(ypos);
		
		m_MouseMouvementVec = { dx, dy };
		//std::cout << "dx: " << dx << " dy: " << dy << std::endl;
	}

	m_DragStart = { xpos, ypos };
	m_LastMovementTime = std::chrono::steady_clock::now();
}

void InputManager::MouseEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
			m_IsLeftMousePressed = true;
		else
			m_IsLeftMousePressed = false;
	}

	if (button == GLFW_MOUSE_BUTTON_RIGHT)
	{
		if (action == GLFW_PRESS)
			m_IsRightMousePressed = true;
		else
			m_IsRightMousePressed = false;
	}
}

void InputManager::CursorEnter(int entered)
{
	if (entered == GLFW_TRUE)
	{
		// Cursor has entered the window; potentially reset or update state
	}
	else
	{
		// Cursor has left the window; reset or keep current state
		m_IsLeftMousePressed = false;
		m_IsRightMousePressed = false;
	}
}