package com.crusades;
import com.crusades.core.input.KeyboardHandler;
import com.crusades.core.window.GlfwWindow;

import static org.lwjgl.glfw.GLFW.*;

public class App {

     public static void main(String[] args) {
        try (GlfwWindow window = new GlfwWindow(1280, 720, "Our super duper game")) {
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
        } catch (Exception e) {
            e.printStackTrace();
        }
     }

}
