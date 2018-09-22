package at.bestsolution.fxgl.sample.renderer.objrenderer;

import at.bestsolution.fxgl.sample.renderer.Renderer;

public class ObjRenderer extends Renderer {

	static {
		Library.require();
	}
	
	private long rendererId;
	
	public ObjRenderer() {
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
