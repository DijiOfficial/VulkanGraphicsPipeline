#include "vulkan_shader.h"
#include "vulkanbase/VulkanUtil.h"
#include <vulkanbase/VulkanBase.h>
#include "mesh/Vertex.h"

void VulkanShader::Initialize()
{
	m_ShaderStages.push_back(CreateShaderInfo(true));
	m_ShaderStages.push_back(CreateShaderInfo(false));
}

void VulkanShader::DestroyShaderModules()
{
	for (const auto& shaderStage : m_ShaderStages)
	{
		vkDestroyShaderModule(VulkanBase::device, shaderStage.module, nullptr);
	}
	m_ShaderStages.clear();
}

VkPipelineShaderStageCreateInfo VulkanShader::CreateShaderInfo(bool isFragment)
{
	std::vector<char> shaderCode = readFile(isFragment ? m_FragmentShaderFile : m_VertexShaderFile);
	VkShaderModule shaderModule = CreateShaderModule(shaderCode);

	VkPipelineShaderStageCreateInfo shaderStageInfo{};
	shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shaderStageInfo.stage = isFragment ? VK_SHADER_STAGE_FRAGMENT_BIT : VK_SHADER_STAGE_VERTEX_BIT;
	shaderStageInfo.module = shaderModule;
	shaderStageInfo.pName = "main";
	return shaderStageInfo;
}

VkPipelineVertexInputStateCreateInfo VulkanShader::CreateVertexInputStateInfo()
{
	static auto bindingDescription = Vertex2D::GetBindingDescription();
	static auto attributeDescriptions = Vertex2D::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	return vertexInputInfo;
}

VkPipelineInputAssemblyStateCreateInfo VulkanShader::CreateInputAssemblyStateInfo()
{
	VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;
	return inputAssembly;
}

VkShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code)
{
	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

	VkShaderModule shaderModule;
	if (vkCreateShaderModule(VulkanBase::device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	return shaderModule;
}