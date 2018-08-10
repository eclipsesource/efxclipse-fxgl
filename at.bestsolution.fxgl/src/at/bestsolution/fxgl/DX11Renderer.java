package at.bestsolution.fxgl;

public class DX11Renderer {

	private static native void nRun(long sharedHandle);
	
	public static void Run(long sharedHandle) {
		System.err.println("DX11Renderer.Run(" + sharedHandle + ")");
		System.loadLibrary("DX11Renderer");
		Thread n = new Thread(() -> {
			nRun(sharedHandle);
		});
		n.setName("DX11Renderer");
		n.start();
	}
	
	public static void main(String[] args) {
		
		Run(1001);
		
	}
}
