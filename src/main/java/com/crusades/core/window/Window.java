package com.crusades.core.window;

public interface Window extends AutoCloseable {
    void clear();
    void clear(float r, float g, float b, float a);
    void render();
    void update();
    boolean isOpen();
}
