package at.bestsolution.fxgl;

import java.net.URL;


public class Library {

	static String NAME = "fxgl";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		NativeHelper.loadLibrary(LIB());
	}
	
}
