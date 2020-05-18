package com.crusades.core.input;

public class KeyboardHandler {

    public static boolean keys[] = new boolean[1024];

    public static boolean isKeyPressed(int keyCode) {
        return keys[keyCode];
    }

    public void keyPressed(int keyCode) {
        keys[keyCode] = true;
    }

    public void keyReleased(int keyCode) {
        keys[keyCode] = false;
    }
}
