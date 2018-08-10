package at.bestsolution.fxgl;

public class DX9Renderer {

	private static native void nRun(long sharedHandle);
	
	public static void Run(long sharedHandle) {
		System.loadLibrary("DX9Renderer");
		Thread n = new Thread(() -> {
			nRun(sharedHandle);
		});
		n.setName("DX9Renderer");
		n.start();
	}
	
	public static void main(String[] args) {
		
		Run(1001);
		
	}
}
