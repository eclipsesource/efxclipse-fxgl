package at.bestsolution.fxgl.sample.renderer.objrenderer;

import java.net.URL;

import at.bestsolution.fxgl.NativeHelper;

public class Library {
                         
	static String NAME = "fxgl_sample_renderer_objrenderer";
	
	private static URL LIB() {
		String mappedName = System.mapLibraryName(NAME);
		URL url = Library.class.getResource("/" + mappedName);
		if (url == null) {
			throw new RuntimeException("library " + mappedName + " not found!!!");
		}
		return url;
	}
	
	public static void require() {
		at.bestsolution.fxgl.sample.renderer.Library.require();
		NativeHelper.loadLibrary(LIB());
		
		NativeHelper.requireResource(Library.class.getResource("/vertex.glsl"));
		NativeHelper.requireResource(Library.class.getResource("/fragment.glsl"));
		NativeHelper.requireResource(Library.class.getResource("/Chest.obj"));
		NativeHelper.requireResource(Library.class.getResource("/Chest.mtl"));
		NativeHelper.requireResource(Library.class.getResource("/Chest-diffuse.tga"));
		NativeHelper.requireResource(Library.class.getResource("/Chest-normal.tga"));
		NativeHelper.requireResource(Library.class.getResource("/Chest-specular.tga"));
	}
	
	
	public static void main(String[] args) {
		require();
		System.err.println("linked = ?");
	}
}
