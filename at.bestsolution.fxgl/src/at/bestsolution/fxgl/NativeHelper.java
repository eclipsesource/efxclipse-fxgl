package at.bestsolution.fxgl;

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
