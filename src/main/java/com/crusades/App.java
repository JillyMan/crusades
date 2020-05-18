package com.crusades;

import com.crusades.core.input.KeyboardHandler;
import com.crusades.core.window.GlfwWindow;
import org.lwjgl.*;
import org.lwjgl.glfw.*;
import org.lwjgl.opengl.*;
import org.lwjgl.system.*;

import java.nio.*;

import static org.lwjgl.glfw.Callbacks.*;
import static org.lwjgl.glfw.GLFW.*;
import static org.lwjgl.opengl.GL11.*;
import static org.lwjgl.system.MemoryStack.*;
import static org.lwjgl.system.MemoryUtil.*;

public class App {

     public static void main(String[] args) throws Exception {
        try(GlfwWindow window = new GlfwWindow(1280, 720, "Our super duper game")) {
            while(window.isOpen()) {
                window.clear();
                window.update();

                if (KeyboardHandler.isKeyPressed(GLFW_KEY_W)) {
                    System.out.println("The key W is pressed");
                }

                if (KeyboardHandler.isKeyPressed(GLFW_KEY_D)) {
                    System.out.println("The key D is pressed");
                }

                if (KeyboardHandler.isKeyPressed(GLFW_KEY_S)) {
                    System.out.println("The key S is pressed");
                }

                if (KeyboardHandler.isKeyPressed(GLFW_KEY_A)) {
                    System.out.println("The key A is pressed");
                }

                window.render();
            }
        }
    }

}
