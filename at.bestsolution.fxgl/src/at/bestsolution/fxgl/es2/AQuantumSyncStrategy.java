package at.bestsolution.fxgl.es2;

import java.lang.reflect.Method;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.function.Supplier;

public abstract class AQuantumSyncStrategy implements ISyncStrategy {

	protected <T> T supplyQuantumRenderer(Supplier<T> supplier) {
		try {
			return CompletableFuture.supplyAsync(supplier, getQuantumRenderer()).get();
		}
		catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	protected void runQuantumRenderer(Runnable runnable) {
		try {
			CompletableFuture.runAsync(runnable, getQuantumRenderer()).get();
		}
		catch (Exception e) {
			throw new RuntimeException(e);
		}
	}
	
	private static ThreadPoolExecutor quantumRenderer;
	protected static ThreadPoolExecutor getQuantumRenderer() {
		if (quantumRenderer == null) {
			try {
				Class<?> quantumRendererClass = Class.forName("com.sun.javafx.tk.quantum.QuantumRenderer");
				Method getInstanceMethod = quantumRendererClass.getDeclaredMethod("getInstance");
				getInstanceMethod.setAccessible(true);
				quantumRenderer = (ThreadPoolExecutor) getInstanceMethod.invoke(null);
				if (quantumRenderer == null) {
					throw new RuntimeException("init failed!");
				}
			}
			catch (Exception e) {
				throw new RuntimeException("init failed", e);
			}
		}
		return quantumRenderer;
	}
}
