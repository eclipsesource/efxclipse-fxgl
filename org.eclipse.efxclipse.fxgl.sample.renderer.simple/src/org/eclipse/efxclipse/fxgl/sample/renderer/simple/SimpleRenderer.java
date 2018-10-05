package org.eclipse.efxclipse.fxgl.sample.renderer.simple;


import org.eclipse.efxclipse.fxgl.sample.renderer.Renderer;

public class SimpleRenderer extends Renderer {

	static {
		Library.require();
	}
	
	private long rendererId;
	
	public SimpleRenderer() {
		rendererId = nCreate();
	}
	
	@Override
	public long getRendererId() {
		return rendererId;
	}
	
	public void dispose() {
		nDestroy(rendererId);
	}
	
	public void LoadObj(String name) {
		nLoadObj(rendererId, name);
	}

	private static native long nCreate();
	private static native void nDestroy(long rendererId);
	private static native void nLoadObj(long rendererId, String name);
	
}
