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
    private final PerformanceWrapper performanceWrapper;

    private App() {
        window = new GlfwWindow(1280, 720, "Our super duper game", 0, System.err);
        performanceWrapper = new PerformanceWrapper(60);
    }

    public void run() {
        while(window.isOpen()) {
            performanceWrapper.run((deltaTime) -> {
                update(deltaTime);
                window.clear(r, g, b, a);
                window.render();
           });
        }
    }

    public void update(float time) {
        window.update();
        if (KeyboardHandler.isKeyPressed(GLFW_KEY_W)) {
            r = rand.nextFloat();
            g = rand.nextFloat();
            b = rand.nextFloat();
        }
    }

    public void close() {
        try {
            window.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

     public static void main(String[] args) {
        var app = new App();
        app.run();
        app.close();
     }

}
