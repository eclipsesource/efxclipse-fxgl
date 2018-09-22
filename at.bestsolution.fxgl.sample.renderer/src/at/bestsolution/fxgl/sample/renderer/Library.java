package at.bestsolution.fxgl.sample.renderer;

import java.net.URL;

import at.bestsolution.fxgl.NativeHelper;

public class Library {
	
	static String NAME = "fxgl_sample_renderer";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		at.bestsolution.fxgl.Library.require();
		at.bestsolution.fxgl.glcommon.Library.require();
		NativeHelper.loadLibrary(LIB());
	}
	
}
