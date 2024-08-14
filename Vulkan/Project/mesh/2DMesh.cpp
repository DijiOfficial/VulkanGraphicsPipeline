#include "2DMesh.h"

Mesh2D::Mesh2D(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout, const std::vector<Vertex2D>& vertices)
	: m_Vertices2D{ vertices }
{
	AllocateBuffer(commandPool, graphicsQueue, descriptorSetLayout);
}

void Mesh2D::AddVertex(const glm::vec2& pos, const glm::vec3& color)
{
	AddVertex(Vertex2D{ pos, color });
}

void Mesh2D::AddVertex(const Vertex2D& vertex)
{
	if (m_VertexIndexUMap.count(vertex) == 0)
	{
		m_VertexIndexUMap[vertex] = static_cast<uint32_t>(m_Vertices2D.size());
		m_Vertices2D.push_back(vertex);
	}
	m_Indices.push_back(m_VertexIndexUMap[vertex]);
}

void Mesh2D::AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout)
{
	if (!m_IsAllocated)
	{
		const VkDeviceSize size = m_Vertices2D.size() * sizeof(m_Vertices2D[0]);
		const VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		const VkDeviceSize Isize = m_Indices.size() * sizeof(m_Indices[0]);
		const VkBufferUsageFlags Iusage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		m_VertexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, size, m_Vertices2D.data(), usage);
		m_IndexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, Isize, m_Indices.data(), Iusage);

		m_DescriptorPool.Initialize(descriptorSetLayout);

		m_IsAllocated = true;
	}
}

void Mesh2D::Destroy()
{
	if (m_IsAllocated)
	{
		m_IndexBuffer->Destroy();
		m_VertexBuffer->Destroy();
		m_IndexBuffer.reset();
		m_VertexBuffer.reset();

		m_DescriptorPool.Destroy();

		m_IsAllocated = false;
	}
}
