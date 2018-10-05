package org.eclipse.efxclipse.fxgl.sample.renderer;

import javafx.animation.AnimationTimer;
import javafx.beans.property.ReadOnlyDoubleWrapper;

public class PerformanceTracker {

	private final AnimationTimer timer;


	private long last = 0;
	private final long updateTime = 1_000_000_000; // ns
	
    private ReadOnlyDoubleWrapper avgFps = new ReadOnlyDoubleWrapper(this, "avgFps");
    
    
    public PerformanceTracker(Renderer renderer) {
    	timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
            	if (last + updateTime < now) {
            		avgFps.set(renderer.getAvgFps());
            		last = now;
            	}
            }
        };
    }
    
    public void start() {
    	timer.start();
    }
    public void stop() {
    	timer.stop();
    }

	public final javafx.beans.property.ReadOnlyDoubleProperty avgFpsProperty() {
		return this.avgFps.getReadOnlyProperty();
	}
	

	public final double getAvgFps() {
		return this.avgFpsProperty().get();
	}
	
	
}
