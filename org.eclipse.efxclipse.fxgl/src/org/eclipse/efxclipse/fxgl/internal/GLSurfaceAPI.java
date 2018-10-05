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
package org.eclipse.efxclipse.fxgl.internal;

import org.eclipse.efxclipse.fxgl.Library;
import org.eclipse.efxclipse.fxgl.es2.GLSurface;

public class GLSurfaceAPI {

	static {
		Library.require();
		//System.loadLibrary("FXGL");
		nInitialize();
	}
	
	public static class ScrollEvent {
		public float deltaX;
		public float deltaY;
	}
	
	private static native long nInitialize();
	
	private static native long nCreateGLSurface(GLSurface surface);
	private static native long nDisposeGLSurface(long surfaceId);
	
	private static native long nFireSizeChanged(long surfaceId);
	private static native void nFireScrollEvent(long surfaceId, ScrollEvent event);
	
	public static long createGLSurface(GLSurface surface) {
		return nCreateGLSurface(surface);
	}
	
	public static void disposeGLSurface(long surfaceId) {
		nDisposeGLSurface(surfaceId);
	}
	
	public static void fireSizeChanged(long surfaceId) {
		nFireSizeChanged(surfaceId);
	}
	
	public static void fireScrollEvent(long surfaceId, ScrollEvent event) {
		nFireScrollEvent(surfaceId, event);
	}
	
}
