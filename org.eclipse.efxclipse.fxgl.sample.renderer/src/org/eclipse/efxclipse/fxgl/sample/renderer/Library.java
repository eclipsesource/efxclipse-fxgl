package org.eclipse.efxclipse.fxgl.sample.renderer;

import java.net.URL;

import org.eclipse.efxclipse.fxgl.NativeHelper;

public class Library {
	
	static String NAME = "fxgl_sample_renderer";
	
	private static URL LIB() {
		return Library.class.getResource("/" + System.mapLibraryName(NAME));
	}
	
	public static void require() {
		org.eclipse.efxclipse.fxgl.Library.require();
		org.eclipse.efxclipse.fxgl.sample.glcommon.Library.require();
		NativeHelper.loadLibrary(LIB());
	}
	
}
