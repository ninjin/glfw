//========================================================================
// Oculus Rift test
// Copyright (c) Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//
// This test is intended to verify Oculus Rift detection and render setup
//
//========================================================================

#if defined(_WIN32)
 #define GLFW_EXPOSE_NATIVE_WIN32
 #define GLFW_EXPOSE_NATIVE_WGL
#elif defined(__APPLE__)
 #define GLFW_EXPOSE_NATIVE_COCOA
 #define GLFW_EXPOSE_NATIVE_NSGL
#elif defined(__linux__)
 #define GLFW_EXPOSE_NATIVE_X11
 #define GLFW_EXPOSE_NATIVE_GLX
#else
 #error "Unsupported platform for Oculus Rift integration"
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <OVR_CAPI.h>

#include <stdio.h>
#include <stdlib.h>

static GLFWmonitor* find_rift(ovrHmd hmd)
{
    int i, count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);

    for (i = 0;  i < count;  i++)
    {
#if defined(_WIN32)
        if (strcmp(glfwGetWin32Monitor(monitors[i]), hmd->DisplayDeviceName) == 0)
            return monitors[i];
#elif defined(__APPLE__)
        if (glfwGetCocoaMonitor(monitors[i]) == hmd->DisplayId)
            return monitors[i];
#elif defined(__linux__)
#endif
    }

    return NULL;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(void)
{
    ovrHmd hmd;
    GLFWmonitor* monitor;
    GLFWwindow* window;
    const GLFWvidmode* mode;

    ovr_Initialize();

    hmd = ovrHmd_Create(0);
    if (!hmd)
        exit(EXIT_FAILURE);

    ovrHmd_ConfigureTracking(hmd, ovrTrackingCap_Orientation,
                                  ovrTrackingCap_Orientation);

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    monitor = find_rift(hmd);
    if (!monitor)
    {
        fprintf(stderr, "No GLFW monitor found that matches Rift");
        exit(EXIT_FAILURE);
    }

    mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    window = glfwCreateWindow(mode->width, mode->height, "Oculus Rift Test", monitor, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    glClearColor(1.0, 0.0, 0.0, 0.0);

    while (!glfwWindowShouldClose(window))
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    ovr_Shutdown();

    exit(EXIT_SUCCESS);
}

