#pragma once
#include <vulkan/vulkan_core.h>
#include <vector>
#include <string>

class VulkanShader final
{
public:
	VulkanShader(const std::string& vertexShaderFile, const std::string& fragmentShaderFile)
		: m_VertexShaderFile{ vertexShaderFile }
		, m_FragmentShaderFile{ fragmentShaderFile }
	{
	};
	~VulkanShader() = default;

	VulkanShader(const VulkanShader& other) = delete;
	VulkanShader(VulkanShader&& other) = delete;
	VulkanShader& operator=(const VulkanShader& other) = delete;
	VulkanShader& operator=(VulkanShader&& other) = delete;

	void Initialize();
	void Destroy();
	void DestoryDescriptorSetLayout();

	std::vector<VkPipelineShaderStageCreateInfo>& GetShaderStages() { return m_ShaderStages; };
	const VkDescriptorSetLayout& GetDescriptorSetLayout() { return m_DescriptorSetLayout; };

	//temp to remove
	static VkPipelineVertexInputStateCreateInfo CreateVertexInputStateInfo();
	VkPipelineInputAssemblyStateCreateInfo CreateInputAssemblyStateInfo();
	static void CreateDescriptorSetLayout();


private:
	std::string m_VertexShaderFile;
	std::string m_FragmentShaderFile;

	std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;

	inline static VkDescriptorSetLayout m_DescriptorSetLayout;

	VkPipelineShaderStageCreateInfo CreateShaderInfo(bool isFragment);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);
};
