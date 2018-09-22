package at.bestsolution.fxgl.glcommon;

import java.net.URL;

import at.bestsolution.fxgl.NativeHelper;

public class Library {
	static String NAME = "fxgl_glcommon";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		NativeHelper.loadLibrary(LIB());
	}
}
