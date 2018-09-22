package at.bestsolution.fxgl;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.net.URL;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.nio.file.StandardCopyOption;
import java.util.HashSet;
import java.util.Set;

public class NativeHelper {

	private static Set<URL> loaded = new HashSet<>();
	
	public static void loadLibrary(URL resource) {
		if (loaded.contains(resource)) {
			System.err.println("NativeHelper: already loaded " + resource);
			return;
		}
		System.err.println("NativeHelper: loading " + resource);
		Path target = Paths.get("").toAbsolutePath();
//		System.err.println("NativeHelper: target = " + target);
	
		Path libFileName = Paths.get(resource.getFile()).getFileName();
		
		//String libFileName = resource.getFile();
		System.err.println("              libFileName = " + libFileName);
		
		Path lib = target.resolve(libFileName);
		try (InputStream in = resource.openStream()) {
			Files.copy(in, lib, StandardCopyOption.REPLACE_EXISTING);
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		
		System.err.println("              " + lib.toString());
		
		System.load(lib.toString());
		NativeHelper.loaded.add(resource);
	}
	
}
