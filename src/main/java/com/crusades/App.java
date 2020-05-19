package com.crusades;

import java.util.Random;
import com.crusades.core.input.KeyboardHandler;
import com.crusades.core.utils.PerformanceWrapper;
import com.crusades.core.window.GlfwWindow;
import com.crusades.core.window.Window;
import static org.lwjgl.glfw.GLFW.*;

public class App {
    private final Random rand = new Random();
    private float r = 0.5f, g = 0.5f, b = 0.5f, a = 1.0f;

    private final Window window;
    private PerformanceWrapper performanceWrapper;

    private App() {
        window = new GlfwWindow(1280, 720, "Our super duper game", 0, System.err);
    }

    public void run() {
        performanceWrapper = new PerformanceWrapper(60, (deltaTime) -> {
            window.update();
            updateGame(deltaTime);
            window.clear(r, g, b, a);
            window.render();
        });

        while(window.isOpen()) {
            performanceWrapper.run();
        }
    }

    public void updateGame(float time) {
        if (KeyboardHandler.isKeyPressed(GLFW_KEY_W)) {
            r = rand.nextFloat();
            g = rand.nextFloat();
            b = rand.nextFloat();
        }
    }

    public void close() {
        window.close();
    }

     public static void main(String[] args) {
        var app = new App();
        app.run();
        app.close();
     }

}
