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
		NativeHelper.loadLibrary(LIB());
	}
	
}
