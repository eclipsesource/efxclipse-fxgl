package org.eclipse.efxclipse.fxgl;

import java.net.URL;

// TODO moveme
public class Library {

	static String NAME = "fxgl";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		NativeHelper.loadLibrary(LIB());
	}
	
}
