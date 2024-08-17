#include "Singleton.h"
#include <unordered_set>
#include <glm/glm.hpp>
#include <chrono>

struct GLFWwindow;
class InputManager final : public Singleton<InputManager>
{
public:
	void Init(GLFWwindow* window);
	glm::vec2 GetMouseMouvementVec() const { return m_MouseMouvementVec; }
	std::unordered_set<int> GetPressedKeys() const { return m_PressedKeys; }
	std::vector<int> GetReleasedKeys();

	bool IsRightMousePressed() const { return m_IsRightMousePressed; }
	bool IsLeftMousePressed() const { return m_IsLeftMousePressed; }
	bool IsMouseIdle() const;

private:
	bool m_IsRightMousePressed = false;
	bool m_IsLeftMousePressed = false;
	std::unordered_set<int> m_PressedKeys;
	std::vector<int> m_ReleasedKeys;
	glm::vec2 m_DragStart{};
	glm::vec2 m_MouseMouvementVec{};
	std::chrono::steady_clock::time_point m_LastMovementTime;
	
	void KeyEvent(int key, int scancode, int action, int mods);
	void MouseMove(GLFWwindow* window, double xpos, double ypos);
	void MouseEvent(GLFWwindow* window, int button, int action, int mods);
	void CursorEnter(int entered);
};