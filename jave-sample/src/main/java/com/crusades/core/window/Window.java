package com.crusades.core.window;

public interface Window {
    void clear();
    void clear(float r, float g, float b, float a);
    void render();
    void update();
    void close();

    boolean isOpen();
}
