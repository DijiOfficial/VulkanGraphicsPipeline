#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>
#include <glm/gtx/hash.hpp>

struct Vertex2D
{
	alignas(16) glm::vec2 m_Pos;
	alignas(16) glm::vec3 m_Color;

	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex2D);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	static std::array<VkVertexInputAttributeDescription, 2> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex2D, m_Pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 2;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex2D, m_Color);

		return attributeDescriptions;
	}

	bool operator==(const Vertex2D& other) const
	{
		return m_Pos == other.m_Pos;
	}

};

namespace std {
	template<> struct hash<Vertex2D>
	{
		size_t operator()(Vertex2D const& vertex) const
		{
			return ((hash<glm::vec2>()(vertex.m_Pos) ^
				(hash<glm::vec3>()(vertex.m_Color) << 1)) >> 1);
		}
	};
}

struct UniformBufferObject
{
	alignas(16) glm::mat4 model;
	alignas(16) glm::mat4 view;
	alignas(16) glm::mat4 proj;
};