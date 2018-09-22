package at.bestsolution.fxgl.internal;

import at.bestsolution.fxgl.Library;
import at.bestsolution.fxgl.es2.GLSurface;

public class GLSurfaceAPI {

	static {
		Library.require();
		//System.loadLibrary("FXGL");
	}
	
	public static class ScrollEvent {
		public float deltaX;
		public float deltaY;
	}
	
	private static native long nCreateGLSurface(GLSurface surface);
	private static native long nDisposeGLSurface(long surfaceId);
	
	private static native long nFireSizeChanged(long surfaceId);
	private static native void nFireScrollEvent(long surfaceId, ScrollEvent event);
	
	private static native float nGetFPS(long surfaceId);
	
	public static float getFPS(long surfaceId) {
		return nGetFPS(surfaceId);
	}
	
	
	public static long createGLSurface(GLSurface surface) {
		return nCreateGLSurface(surface);
	}
	
	public static void disposeGLSurface(long surfaceId) {
		nDisposeGLSurface(surfaceId);
	}
	
	
	public static void fireSizeChanged(long surfaceId) {
		nFireSizeChanged(surfaceId);
	}
	
	public static void fireScrollEvent(long surfaceId, ScrollEvent event) {
		nFireScrollEvent(surfaceId, event);
	}
	
}
