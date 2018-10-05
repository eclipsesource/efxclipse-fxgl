package org.eclipse.efxclipse.fxgl.sample.renderer.simple;


import java.net.URL;

import org.eclipse.efxclipse.fxgl.NativeHelper;

public class Library {
                         
	static String NAME = "fxgl_sample_renderer_simple";
	
	private static URL LIB() {
		String mappedName = System.mapLibraryName(NAME);
		URL url = Library.class.getResource("/" + mappedName);
		if (url == null) {
			throw new RuntimeException("library " + mappedName + " not found!!!");
		}
		return url;
	}
	
	public static void require() {
		org.eclipse.efxclipse.fxgl.sample.renderer.Library.require();
		NativeHelper.loadLibrary(LIB());
		
		NativeHelper.requireResource(Library.class.getResource("/Triangle.vertexshader"));
		NativeHelper.requireResource(Library.class.getResource("/Triangle.fragmentshader"));
	}
	
	
	public static void main(String[] args) {
		require();
		System.err.println("linked = ?");
	}
}
