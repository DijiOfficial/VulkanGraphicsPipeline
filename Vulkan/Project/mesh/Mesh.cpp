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
		m_IndexBuffer->Destroy();
		m_VertexBuffer->Destroy();
		m_IndexBuffer.reset();
		m_VertexBuffer.reset();
		m_IsAllocated = false;
	}
}

void Mesh::Draw(const VkCommandBuffer& commandBuffer) const
{
	if (!m_IsAllocated) return;

	BindBuffers(commandBuffer);

	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

void Mesh::AddVertex(const glm::vec2& pos, const glm::vec3& color)
{
	AddVertex(Vertex2D{ pos, color });
}

void Mesh::AddVertex(const Vertex2D& vertex)
{
	if (m_VertexIndexUMap.count(vertex) == 0)
	{
		m_VertexIndexUMap[vertex] = static_cast<uint32_t>(m_Vertices.size());
		m_Vertices.push_back(vertex);
	}
	m_Indices.push_back(m_VertexIndexUMap[vertex]);
}

void Mesh::AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue)
{
	if (!m_IsAllocated)
	{
		const VkDeviceSize size = m_Vertices.size() * sizeof(m_Vertices[0]);
		const VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		const VkDeviceSize Isize = m_Indices.size() * sizeof(m_Indices[0]);
		const VkBufferUsageFlags Iusage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		m_VertexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, size, m_Vertices.data(), usage);
		m_IndexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, Isize, m_Indices.data(), Iusage);

		m_IsAllocated = true;
	}
}

void Mesh::BindBuffers(const VkCommandBuffer& commandBuffer) const
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer->GetDataBuffer() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetDataBuffer(), 0, VK_INDEX_TYPE_UINT32);

}
