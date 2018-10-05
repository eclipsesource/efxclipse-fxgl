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
package org.eclipse.efxclipse.fxgl;

import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.HashSet;
import java.util.Set;

// TODO moveme
public class NativeHelper {

	private static Set<URL> loaded = new HashSet<>();
	
	private static Path extractResource(URL resource) {
		Path target = Paths.get("").toAbsolutePath();
		Path fileName = Paths.get(resource.getFile()).getFileName();
		Path extracted = target.resolve(fileName);
		try (InputStream in = resource.openStream()) {
			Files.copy(in, extracted, StandardCopyOption.REPLACE_EXISTING);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		return extracted;
	}
	
	public static void loadLibrary(URL resource) {
		if (loaded.contains(resource)) {
			return;
		}
		Path lib = extractResource(resource);
		System.load(lib.toString());
		NativeHelper.loaded.add(resource);
	}
	
	public static void requireResource(URL resource) {
		if (loaded.contains(resource)) {
			return ;
		}
		extractResource(resource);
		NativeHelper.loaded.add(resource);
	}
	
}
