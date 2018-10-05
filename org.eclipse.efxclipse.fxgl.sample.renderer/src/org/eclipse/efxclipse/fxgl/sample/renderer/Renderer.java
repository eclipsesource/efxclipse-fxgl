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

import org.eclipse.efxclipse.fxgl.es2.GLSurface;
import org.eclipse.efxclipse.fxgl.sample.renderer.internal.RendererAPI;

public abstract class Renderer {
	
	static {
		Library.require();
	}
	
	public abstract long getRendererId();
	
	public void setRenderTarget(GLSurface surface) {
		RendererAPI.nSetRenderTarget(getRendererId(), surface.getSurfaceId());
	}
	public void setTargetFramerate(float framerate) {
		RendererAPI.nSetTargetFramerate(getRendererId(), framerate);
	}
	
	private boolean running = false;
	
	public void start() {
		RendererAPI.nStartRenderer(getRendererId());
		running = true;
	}
	public void stop() {
		RendererAPI.nStopRenderer(getRendererId());
		running = false;
	}
	
	public boolean isRunning() {
		return running;
	}
	
	public double getAvgFps() {
		return RendererAPI.nGetAvgFps(getRendererId());
	}
	
	public void setTargetFrameRate(double fps) {
		RendererAPI.nSetTargetFramerate(getRendererId(), (float) fps);
	}
}
