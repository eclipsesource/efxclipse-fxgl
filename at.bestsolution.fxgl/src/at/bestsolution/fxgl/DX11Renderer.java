package at.bestsolution.fxgl;

public class DX11Renderer {

	private static native void nRun(long sharedHandle);
	
	public static void Run(long sharedHandle) {
		System.loadLibrary("DX11Renderer");
		Thread n = new Thread(() -> {
			nRun(sharedHandle);
		});
		n.setName("DX11Renderer");
		n.start();
	}
}
