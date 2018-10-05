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
package org.eclipse.efxclipse.fxgl.es2;

import java.util.function.BiFunction;

import org.eclipse.efxclipse.fxgl.es2.NGGLSurface.Tex;

public interface ISyncStrategy {
	Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex);
	void SwapTex();
	void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex);
	void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex);
	
	Tex getRenderTex();
	
	
	default boolean checkSize(int w, int h, Tex tex) {
		return tex.w == w && tex.h == h;
	}
}
