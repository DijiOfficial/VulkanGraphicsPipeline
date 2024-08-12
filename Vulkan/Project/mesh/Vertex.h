#pragma once
#include <vulkan/vulkan_core.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

struct Vertex2D
{
	glm::vec2 m_Pos;
	glm::vec3 m_Color;

	//static VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo()
	//{
	//	static auto bindingDescription = Vertex2D::GetBindingDescription();
	//	static auto attributeDescriptions = Vertex2D::GetAttributeDescriptions();

	//	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	//	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	//	vertexInputInfo.vertexBindingDescriptionCount = 1;
	//	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	//	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	//	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	//	return vertexInputInfo;
	//}

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