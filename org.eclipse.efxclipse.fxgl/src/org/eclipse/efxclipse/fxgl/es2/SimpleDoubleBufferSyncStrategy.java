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

import java.util.concurrent.atomic.AtomicReference;
import java.util.function.BiFunction;

import org.eclipse.efxclipse.fxgl.es2.NGGLSurface.Tex;

public class SimpleDoubleBufferSyncStrategy implements ISyncStrategy {

	private class DoubleBuffer {
		Tex foreground;
		Tex background;
	}
	
	private AtomicReference<DoubleBuffer> buffer = new AtomicReference<>();
	
	@Override
	public Tex GetNextTex(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		return buffer.get().background;
	}

	@Override
	public void SwapTex() {
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			newBuf.foreground = buf.background;
			newBuf.background = buf.foreground;
			return newBuf;
		});
	}

	@Override
	public void beforeRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		// init
		buffer.updateAndGet(buf -> {
			if (buf == null) {
				DoubleBuffer newBuf = new DoubleBuffer();
				newBuf.foreground = createTex.apply(w, h);
				newBuf.background = createTex.apply(w, h);
				return newBuf;
			}
			return buf;
		});
	}
	@Override
	public void afterRenderContent(int w, int h, BiFunction<Integer, Integer, Tex> createTex) {
		buffer.updateAndGet(buf -> {
			DoubleBuffer newBuf = new DoubleBuffer();
			// keep foreground
			newBuf.foreground = buf.foreground;
			
			// prepare background
			newBuf.background = checkSize(w, h, buf.background) ? buf.background : createTex.apply(w, h);
			return newBuf;
		});
	}

	@Override
	public Tex getRenderTex() {
		return buffer.get().foreground;
	}

}
