package org.eclipse.efxclipse.fxgl.sample.renderer.objrenderer;

public class Testcode {

	public static void main(String[] args) {
		
		ObjRenderer renderer = new ObjRenderer();
		
		System.err.println("created ObjRenderer " + renderer.getRendererId());
		
		renderer.start();
		
		
		
	}
}
