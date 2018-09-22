package at.bestsolution.fxgl.sample.application;
import com.sun.javafx.perf.PerformanceTracker;

import javafx.animation.AnimationTimer;
import javafx.application.Application;
import javafx.beans.property.DoubleProperty;
import javafx.beans.property.ReadOnlyDoubleProperty;
import javafx.beans.property.ReadOnlyDoubleWrapper;
import javafx.beans.property.SimpleDoubleProperty;
import javafx.scene.Scene;
import javafx.scene.control.Label;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;

public class Performance {

	private final PerformanceTracker tracker;
	private final AnimationTimer timer;
	
	private long last = 0;
	private final long updateTime = 1_000_000_000; // ns

    private ReadOnlyDoubleWrapper avgFps = new ReadOnlyDoubleWrapper(this, "avgFps");
    private ReadOnlyDoubleWrapper instantFps = new ReadOnlyDoubleWrapper(this, "instantFps");
    
    
    public Performance(Scene scene) {
    	tracker = PerformanceTracker.getSceneTracker(scene);
    	timer = new AnimationTimer() {
            @Override
            public void handle(long now) {
            	if (last + updateTime < now) {
            		avgFps.set(tracker.getAverageFPS());
            		instantFps.set(tracker.getInstantFPS());
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
	

	public final javafx.beans.property.ReadOnlyDoubleProperty instantFpsProperty() {
		return this.instantFps.getReadOnlyProperty();
	}
	

	public final double getInstantFps() {
		return this.instantFpsProperty().get();
	}
	


	
}