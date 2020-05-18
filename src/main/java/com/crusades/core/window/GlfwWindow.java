package com.crusades.core.window;
import java.io.PrintStream;

import com.crusades.core.input.KeyboardHandler;
import org.lwjgl.glfw.GLFWErrorCallback;
import org.lwjgl.opengl.GL;

import static org.lwjgl.glfw.Callbacks.glfwFreeCallbacks;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.opengl.GL11.GL_DEPTH_BUFFER_BIT;
import static org.lwjgl.system.MemoryUtil.*;

public class GlfwWindow implements Window, AutoCloseable {

    private int width;
    private int height;
    private long windowHandle;
    private final KeyboardHandler input = new KeyboardHandler();

    public GlfwWindow(int width, int height, String title) {
        this(width, height, title, 1, System.err);
    }

    public GlfwWindow(int width, int height, String title, int vsync, PrintStream logStream) {
        this.width = width;
        this.height = height;
        GLFWErrorCallback.createPrint(logStream).set();

        initGlfw(vsync, title);
        initCallbacks();
    }

    @Override
    public void clear() {
        glClearColor(1.0f, 0.5f, 0.5f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    @Override
    public void render() {
        glfwSwapBuffers(windowHandle); // swap the color buffers
    }

    @Override
    public void update() {
        glfwPollEvents();
    }

    @Override
    public boolean isOpen() {
        return !glfwWindowShouldClose(windowHandle);
    }

    @Override
    public void close() throws Exception {
        // Free the window callbacks and destroy the window
        glfwFreeCallbacks(windowHandle);
        glfwDestroyWindow(windowHandle);

        // Terminate GLFW and free the error callback
        glfwTerminate();
        glfwSetErrorCallback(null).free();
    }

    private void initGlfw(int vsync, String title) {
        if (!glfwInit()) {
            throw new IllegalStateException("Unable to initialize GLFW");
        }

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        windowHandle = glfwCreateWindow(width, height, title, NULL, NULL);
        if (windowHandle == NULL)
            throw new RuntimeException("Failed to create the GLFW window");

        glfwMakeContextCurrent(windowHandle);
        glfwSwapInterval(vsync);
        glfwShowWindow(windowHandle);

        // This line is critical for LWJGL's interoperation with GLFW's
        // OpenGL context, or any context that is managed externally.
        // LWJGL detects the context that is current in the current thread,
        // creates the GLCapabilities instance and makes the OpenGL
        // bindings available for use.
        GL.createCapabilities();
    }

    private void initCallbacks() {
        glfwSetKeyCallback(windowHandle, (window, key, scancode, action, mods) -> {
            if (action == GLFW_RELEASE) {
                input.keyReleased(key);
            }

            if (action == GLFW_PRESS) {
                input.keyPressed(key);
            }
        });
    }
}
