/*
 * Copyright (c) 2018 BestSolution and Others. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0, which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception, which is available at
 * https://www.gnu.org/software/classpath/license.html.
 *
 * SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
 */
package org.eclipse.efxclipse.fxgl.sample.application;
import com.sun.javafx.perf.PerformanceTracker;

import javafx.animation.AnimationTimer;
import javafx.beans.property.ReadOnlyDoubleWrapper;
import javafx.scene.Scene;

@SuppressWarnings("restriction")
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