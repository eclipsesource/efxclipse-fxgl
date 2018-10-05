package org.eclipse.efxclipse.fxgl.sample.glcommon;

import java.net.URL;

import org.eclipse.efxclipse.fxgl.NativeHelper;

public class Library {
	static String NAME = "fxgl_sample_glcommon";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		NativeHelper.loadLibrary(LIB());
	}
}
