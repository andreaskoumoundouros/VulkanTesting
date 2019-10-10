#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>

const int WIDTH = 800;
const int HEIGHT = 600;

class HelloTriangleApplication {
public:
	void run() {
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VkInstance instance;

	void initWindow() {
		// Initialize the window.
		glfwInit();

		// We don't want to create an openGL context.
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// Don't allow the window to be resizable.
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		// Create the window with set width, height, and title and leave 
		// monitor selection and openGL specfic parameter as nullptr.
		window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	}

	void initVulkan() {
		createInstance();
	}

	void createInstance() {
		// Create an application info struct with general application info.
		// Not mandatory, though quite useful and may offer optimization
		// benefits as the driver will have more information.
		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = "Hello Triangle";
		appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);		
		appInfo.pEngineName = "No Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_0;

		// Creation info, mandatory.
		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;

		// Extensions for window interaction because Vulkan is 
		// platform independant.
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		
		if (!validateExtensions(glfwExtensions, glfwExtensionCount)) {
			throw std::runtime_error("An extension returned by glfwGetRequiredInstanceExtensions is not included in supported extension list.");
		}

		createInfo.enabledExtensionCount = glfwExtensionCount;
		createInfo.ppEnabledExtensionNames = glfwExtensions;

		createInfo.enabledLayerCount = 0;

		// Finally, create the Vulkan instance with the information above.
		VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

		if (result != VK_SUCCESS) {
			throw std::runtime_error("Failed to create Vulkan instance!");
		}
	}

	bool validateExtensions(const char** glfwExtensions, uint32_t glfwExtensionCount) {
		uint32_t extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> extensions(extensionCount);

		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		for (size_t i = 0; i < glfwExtensionCount; i++)
		{
			bool validated = false;

			for (const auto& extension : extensions) {
				if (std::strcmp(extension.extensionName, glfwExtensions[i]) == 0) {
					validated = true;
					break;
				}
			}
			
			if (!validated) {
				return false;
			}
		}

		return true;
	}

	void mainLoop() {
		// Main looping where frame drawing occurs.
		while (!glfwWindowShouldClose(window)) {
			// Pretty self explanitory, polls for events.
			glfwPollEvents();
		}
	}

	void cleanup() {
		// Destroy the Vulkan instance that we created.
		vkDestroyInstance(instance, nullptr);

		// Pair to glfwCreateWindow.
		glfwDestroyWindow(window);

		// Pair to glfwInit.
		glfwTerminate();
	}
};

int main() {
	HelloTriangleApplication app;

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}