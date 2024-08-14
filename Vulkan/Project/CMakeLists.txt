# Source files
set(SHADER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/shaders")
set(SHADER_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/shaders")

file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${SHADER_SOURCE_DIR}/*.frag"
    "${SHADER_SOURCE_DIR}/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
    get_filename_component(FILE_NAME ${GLSL} NAME)
    set(SPIRV "${SHADER_BINARY_DIR}/${FILE_NAME}.spv")
    add_custom_command(
        OUTPUT ${SPIRV}
        COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SPIRV}
        DEPENDS ${GLSL}
    )
    list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders 
    DEPENDS ${SPIRV_BINARY_FILES}
)


set(SOURCES
    "main.cpp"
    "vulkanbase/VulkanBase.h" 
    "vulkanbase/VulkanUtil.h"
    # Add other source files here
    
    "labwork/Week05.cpp"
    "labwork/Week06.cpp"
 "vulkanbase/VulkanBase.cpp" "mesh/Utils.h" "abstractions/CommandPool.cpp" "abstractions/CommandPool.h" "abstractions/CommandBuffer.h" "abstractions/CommandBuffer.cpp" "abstractions/DataBuffer.h" "abstractions/DataBuffer.cpp"  "Singleton.h" "mesh/Mesh.h" "mesh/Mesh.inl" "Scene.h" "Scene.cpp" "abstractions/RenderPass.h" "abstractions/RenderPass.cpp" "abstractions/SwapChainImageView.h" "abstractions/SwapChainImageView.cpp" "abstractions/GraphicsPipeline.h" "abstractions/GraphicsPipeline.cpp" "abstractions/DescriptorPool.h" "abstractions/DescriptorPool.cpp" "abstractions/DepthBuffer.h" "abstractions/DepthBuffer.cpp" "textures/Textures.h" "textures/Textures.cpp")

# Create the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${GLSL_SOURCE_FILES}  "vulkanbase/VulkanUtil.cpp"  "shaders/vulkan_shader.h" "shaders/vulkan_shader.cpp")
add_dependencies(${PROJECT_NAME} Shaders)
# Link libraries
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Vulkan_LIBRARIES} glfw)