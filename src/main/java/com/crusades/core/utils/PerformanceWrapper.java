package com.crusades.core.utils;

public class PerformanceWrapper {
    private final float framesPerMill;
    private long time, currentTime;
    private int frames, updates;
    private float delta;

    public PerformanceWrapper(int targetFps) {
        framesPerMill = 1_000.0f / targetFps;
        time = currentTime = System.currentTimeMillis();
    }

    public void run(Action action) {
        long nowTime = System.currentTimeMillis();
        delta += nowTime - currentTime;
        currentTime = nowTime;
        if (delta > framesPerMill) {
            action.perform(delta);
            delta = 0.0f;
            updates++;
        }
        frames++;

        if (System.currentTimeMillis() - time > 1000) {
            time += 1000;
            System.out.println("Frames: " + frames + ", Updates: " + updates);
            frames = updates = 0;
        }
    }
}
