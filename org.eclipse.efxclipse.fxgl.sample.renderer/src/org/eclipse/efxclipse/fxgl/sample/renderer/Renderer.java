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
