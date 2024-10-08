#include "Mesh.h"

//temp
#include "core/Camera.h"
#include "core/TimeSingleton.h"

template<typename VertexType>
void Mesh<VertexType>::Destroy()
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

template<typename VertexType>
void Mesh<VertexType>::Update(uint32_t currentFrame, UniformBufferObject ubo)
{
	if (std::is_same<VertexType, Vertex2D>::value)
	{
		ubo.model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationAngle), Camera::UP);
	}
	else
	{
		const float time = diji::TimeSingleton::GetInstance().GetDeltaTime();
		m_ElapsedTime += time;

		// Apply rotation around the UP vector over time
		glm::mat4 rotationMatrix = glm::rotate(m_RotationMatrix, glm::radians(m_ElapsedTime * 90.0f), Camera::UP);
		if (!m_Rotate)
		{
			rotationMatrix = m_RotationMatrix;
		}
		ubo.model = m_ScaleMatrix * rotationMatrix * m_TranslationMatrix;
	}

	m_DescriptorPool.UpdateUniformBuffer(currentFrame, ubo);
}

template<typename VertexType>
void Mesh<VertexType>::Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout& pipelineLayout, uint32_t currentFrame) const
{
	if (!m_IsAllocated) return;

	BindBuffers(commandBuffer);

	vkCmdPushConstants(
		commandBuffer, pipelineLayout,
		VK_SHADER_STAGE_FRAGMENT_BIT, // Stage flag should match the push constant range in the layout
		sizeof(glm::vec3), // Offset within the push constant block
		sizeof(int), // Size of the push constants to update
		&m_HasNormalMap // Pointer to the data
		);

	vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &m_DescriptorPool.GetDescriptorSets(currentFrame), 0, nullptr);
	vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(m_Indices.size()), 1, 0, 0, 0);
}

template<typename VertexType>
void Mesh<VertexType>::AddVertex(const VertexType& vertex)
{
	if (m_VertexIndexUMap.find(vertex) == m_VertexIndexUMap.end())
	{
		m_Vertices.push_back(vertex);
		m_VertexIndexUMap[vertex] = static_cast<uint32_t>(m_Vertices.size() - 1);
	}
	m_Indices.push_back(m_VertexIndexUMap[vertex]);
}

template<typename VertexType>
void Mesh<VertexType>::AllocateBuffer(const VkCommandPool& commandPool, const VkQueue& graphicsQueue, const VkDescriptorSetLayout& descriptorSetLayout)
{
	if (!m_IsAllocated)
	{
		const VkDeviceSize size = m_Vertices.size() * sizeof(m_Vertices[0]);
		const VkBufferUsageFlags usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;

		const VkDeviceSize Isize = m_Indices.size() * sizeof(m_Indices[0]);
		const VkBufferUsageFlags Iusage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT;

		m_VertexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, size, m_Vertices.data(), usage);
		m_IndexBuffer = std::make_unique<DataBuffer>(commandPool, graphicsQueue, Isize, m_Indices.data(), Iusage);

		m_DescriptorPool.Initialize(commandPool, graphicsQueue, descriptorSetLayout);

		m_IsAllocated = true;
	}
}

template<typename VertexType>
void Mesh<VertexType>::BindBuffers(const VkCommandBuffer& commandBuffer) const
{
	VkBuffer vertexBuffers[] = { m_VertexBuffer->GetDataBuffer() };
	VkDeviceSize offsets[] = { 0 };

	vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
	vkCmdBindIndexBuffer(commandBuffer, m_IndexBuffer->GetDataBuffer(), 0, VK_INDEX_TYPE_UINT32);
}