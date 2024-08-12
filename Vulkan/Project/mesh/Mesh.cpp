#include "Mesh.h"

Mesh::Mesh(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const std::vector<Vertex2D>& vertices)
	: m_Vertices{ vertices }
{
	AllocateBuffer(commandPool, graphicsQueue);
}

void Mesh::Destroy()
{
	if (m_IsAllocated)
	{
		m_VertexBuffer->Destroy();
		m_VertexBuffer.reset();
		m_IsAllocated = false;
	}
}

void Mesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	if (!m_IsAllocated) return;

	VkBuffer vertexBuffers[] = { m_VertexBuffer->GetVertexBuffer() };
	VkDeviceSize offsets[] = { 0 };
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);

	vkCmdDraw(commandBuffer, static_cast<uint32_t>(m_Vertices.size()), 1, 0, 0);
}

void Mesh::AddVertex(const glm::vec2& pos, const glm::vec3& color)
{
	AddVertex(Vertex2D{ pos, color });
}

void Mesh::AddVertex(const Vertex2D& vertex)
{
	auto it = std::find(m_Vertices.begin(), m_Vertices.end(), vertex);
	
	m_Vertices.push_back(vertex);

	//if (it == m_Vertices.end())
	//{
	//	m_Vertices.push_back(vertex);
	////	m_Indices.push_back(static_cast<uint32_t>(m_Vertices.size() - 1));
	//}
	//else
	//{
	//	// Vertex is found, use the existing index
	//	m_Indices.push_back(static_cast<uint32_t>(std::distance(m_Vertices.begin(), it)));
	//}
}

void Mesh::AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue)
{
	if (!m_IsAllocated)
	{
		const VkDeviceSize size = m_Vertices.size() * sizeof(m_Vertices[0]);
		const VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		const VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

		m_VertexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, size, m_Vertices.data(), usage, properties);
		m_IsAllocated = true;
	}
}