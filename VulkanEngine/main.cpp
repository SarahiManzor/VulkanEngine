#define GLFW_INCLUDE_VULKAN // This define is needed for glfw3.h to import vulkan/vulkan.h
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <vector>

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
private:
	GLFWwindow* window;
	VkInstance instance; // Handles connection between application and vulkan library

public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	void initWindow() {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // Tells GLFW to not create an OpenGL context
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Self explanatory

		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void hasExensions(const std::vector<VkExtensionProperties> &availableExtensions, const char** neededExtensions, int16_t neededExtensionsCount) {
		std::cout << "Available extensions:\n";
		for (const auto& extension : availableExtensions) {
			std::cout << '\t' << extension.extensionName << '\n';
		}

		std::cout << "Needed extensions:\n";
		for (auto i = 0; i < neededExtensionsCount; i++) {
			const char *neededExtension = neededExtensions[i];
			bool foundMatch = false;
			for (const auto& extension : availableExtensions) {
				if (strcmp(neededExtension, extension.extensionName)) {
					std::cout << '\t' << neededExtension << " FOUND" << '\n';
					foundMatch = true;
					break;
				}
			}
			if (!foundMatch) {
				std::cout << '\t' << neededExtension << " NOT FOUND" << '\n';
			}
		}
	}

	void createInstance() {
		// More about this struct here: https://www.khronos.org/registry/vulkan/specs/1.3-extensions/man/html/VkApplicationInfo.html
		VkApplicationInfo appInfo{};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Create VKInstanceCreateInfo
		VkInstanceCreateInfo createInfo{};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;
		createInfo.enabledLayerCount = 0;

		// Create VkInstance
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		hasExensions(extensions, glfwExtensions, glfwExtensionCount);

		if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create instance!");
		}
	}

	void initVulkan() {
		createInstance();
	}

	void mainLoop() {
		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();
		}
	}

	void cleanup() {
		vkDestroyInstance(instance, nullptr);
		glfwDestroyWindow(window);
		glfwTerminate();
	}
};

int main() {
	HelloTriangleApplication app;
	try {
		app.run();
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}