/*
 * Copyright (c) 2018 BestSolution and Others. All rights reserved.
 *
 * This program and the accompanying materials are made available under the
 * terms of the Eclipse Public License v. 2.0, which is available at
 * http://www.eclipse.org/legal/epl-2.0.
 *
 * This Source Code may also be made available under the following Secondary
 * Licenses when the conditions for such availability set forth in the
 * Eclipse Public License v. 2.0 are satisfied: GNU General Public License,
 * version 2 with the GNU Classpath Exception, which is available at
 * https://www.gnu.org/software/classpath/license.html.
 *
 * SPDX-License-Identifier: EPL-2.0 OR GPL-2.0 WITH Classpath-exception-2.0
 */
package org.eclipse.efxclipse.fxgl.sample.renderer.objrenderer;

import java.net.URL;

import org.eclipse.efxclipse.fxgl.NativeHelper;

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
		org.eclipse.efxclipse.fxgl.sample.renderer.Library.require();
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
