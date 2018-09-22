package at.bestsolution.fxgl.sample.renderer.internal;

import at.bestsolution.fxgl.sample.renderer.Library;

public class RendererAPI {

	static {
		Library.require();
		nInitializeLibrary();
	}
	
	public static native void nInitializeLibrary();
	public static native void nDisposeLibrary();
	
	public static native void nStartRenderer(long rendererId);
	public static native void nStopRenderer(long rendererId);
	public static native void nSetTargetFramerate(long rendererId, float framerate);
	public static native void nSetRenderTarget(long rendererId, long surfaceId);
	public static native double nGetAvgFps(long rendererId);
	
	public static void main(String[] args) {
		System.err.println("online!");
	}
}
