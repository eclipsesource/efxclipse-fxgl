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
