package at.bestsolution.fxgl.es2;

public class GL33Renderer {

	private static native void nRun(long contextHandle, int textureId);
	
	public static void Run(long contextHandle, int textureId) {
		System.err.println("GL33Renderer.Run(" + contextHandle + " , " + textureId + ")");
		System.loadLibrary("GL33Renderer");
		Thread n = new Thread(() -> {
			nRun(contextHandle, textureId);
		});
		n.setName("GL33Renderer");
		n.start();
	}
	
	public static void main(String[] args) {
		
		Run(0, 0);
		
	}
}
