package at.bestsolution.fxgl.sample.application;

import java.util.HashSet;
import java.util.Set;
import java.util.SplittableRandom;
import java.util.stream.IntStream;

import at.bestsolution.fxgl.es2.GLSurface;
import at.bestsolution.fxgl.sample.renderer.PerformanceTracker;
import at.bestsolution.fxgl.sample.renderer.objrenderer.ObjRenderer;
import javafx.application.Application;
import javafx.beans.binding.Bindings;
import javafx.geometry.HPos;
import javafx.geometry.Insets;
import javafx.geometry.Orientation;
import javafx.geometry.VPos;
import javafx.scene.Node;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.Label;
import javafx.scene.control.ListCell;
import javafx.scene.control.SplitPane;
import javafx.scene.control.ToolBar;
import javafx.scene.layout.BorderPane;
import javafx.scene.layout.GridPane;
import javafx.scene.layout.HBox;
import javafx.scene.layout.Priority;
import javafx.stage.Stage;

public class MultiObjectRenderer extends Application {

	private Performance fxPerformance;
	
	private Set<PerformanceTracker> cTrackers = new HashSet<>();
	
	private Node createRenderer() {
		GLSurface surface = new GLSurface();
		
		ObjRenderer renderer = new ObjRenderer();
		renderer.setRenderTarget(surface);
		
		GridPane layout = new GridPane();
		
		ToolBar tb = new ToolBar();
		tb.setStyle("-fx-background-color: transparent");
		{
			Button b = new Button("▶");
			b.setOnAction(e -> {
				if (renderer.isRunning()) {
					renderer.stop();
					
					b.setText("▶");
				}
				else {
					renderer.start();
					b.setText("■");
				}
			});
			tb.getItems().add(b);
		}
		{
			ComboBox<Double> frameRate = new ComboBox<>();
//			frameRate.setCellFactory(listView -> new ListCell<Double>() {
//	            @Override
//	            public void updateItem(Double item, boolean empty) {
//	                super.updateItem(item, empty);
//	                if (empty) {
//	                    setText(null);
//	                    setDisable(false);
//	                } else {
//	                    setText(String.format("%5.0f", item));
//	                }
//	            }
//	        });
			frameRate.getItems().addAll(1d, 5d, 10d, 30d, 60d, 100d, 200d);
			frameRate.setValue(60d);
			frameRate.valueProperty().addListener((x, o, n) -> {
				renderer.setTargetFrameRate(n);
			});
			tb.getItems().add(frameRate);
		}
		{
			Label fps = new Label();
			PerformanceTracker tracker = new PerformanceTracker(renderer);
			cTrackers.add(tracker);
			fps.textProperty().bind(tracker.avgFpsProperty().asString("%5.2ffps"));
			tracker.start();
			tb.getItems().add(fps);
		}
		
		layout.getChildren().setAll(surface, tb);
		GridPane.setConstraints(surface, 0, 0, 1, 1, HPos.CENTER, VPos.CENTER, Priority.ALWAYS, Priority.ALWAYS, Insets.EMPTY);
		GridPane.setConstraints(tb, 0, 0, 1, 1, HPos.CENTER, VPos.TOP, Priority.ALWAYS, Priority.NEVER, Insets.EMPTY);
		
		return layout;
	}
	
	@Override
	public void start(Stage primaryStage) throws Exception {
		
		GLSurface backgroundSurface = new GLSurface();
		ObjRenderer backgroundRenderer = new ObjRenderer();
		backgroundRenderer.setRenderTarget(backgroundSurface);
		ToolBar tb = new ToolBar();
		{
			Button b = new Button("Start Background");
			b.setOnAction(e -> backgroundRenderer.start());
			tb.getItems().add(b);
		}
		{
			Button b = new Button("Stop Background");
			b.setOnAction(e -> backgroundRenderer.stop());
			tb.getItems().add(b);
		}
		
		
		
		
		SplitPane hSplit = new SplitPane();
		hSplit.setStyle("-fx-background-color: transparent");
		hSplit.setOrientation(Orientation.HORIZONTAL);
		
		int numHorizontal = 4;
		int numVertical = 4;
		
		double vOne = 1.0d / numVertical;
		double[] vPositions = IntStream.range(1, numVertical).mapToDouble(i -> i * vOne).toArray();
		
		
		for (int i = 0; i < numHorizontal; i++) {
			
			SplitPane vSplit = new SplitPane();
			vSplit.setStyle("-fx-background-color: transparent");
			vSplit.setOrientation(Orientation.VERTICAL);
			
			for (int j = 0; j < numVertical; j++) {
				Node node = createRenderer();
				vSplit.getItems().add(node);
				SplitPane.setResizableWithParent(node, true);
			}
			
			vSplit.setDividerPositions(vPositions);
			
			hSplit.getItems().add(vSplit);
			SplitPane.setResizableWithParent(vSplit, true);
		}
		
		double hOne = 1.0d / numHorizontal;
		double[] hPositions = IntStream.range(1, numHorizontal).mapToDouble(i -> i * hOne).toArray();
		hSplit.setDividerPositions(hPositions);
		
		
		
		GridPane root = new GridPane();
		root.setPrefSize(1024, 768);
		root.getChildren().setAll(backgroundSurface, hSplit, tb);
		GridPane.setConstraints(hSplit, 0, 1, 1, 1, HPos.CENTER, VPos.CENTER, Priority.ALWAYS, Priority.ALWAYS, Insets.EMPTY);
		GridPane.setConstraints(backgroundSurface, 0, 1, 1, 1, HPos.CENTER, VPos.CENTER, Priority.ALWAYS, Priority.ALWAYS, Insets.EMPTY);
		GridPane.setConstraints(tb, 0, 0);
		Scene scen = new Scene(root);
		
		scen.getStylesheets().add(getClass().getResource("style.css").toExternalForm());
		
		
		primaryStage.setScene(scen);
		primaryStage.show();
		
		HBox spacer = new HBox();
		HBox.setHgrow(spacer, Priority.ALWAYS);
		tb.getItems().add(spacer);
		
		Label fps = new Label();
		tb.getItems().add(fps);
		
		fxPerformance = new Performance(scen);
		fps.textProperty().bind(
				Bindings.concat(
						"JavaFx (",
						fxPerformance.avgFpsProperty().asString("avg: %5.2ffps"),
						", ",
						fxPerformance.instantFpsProperty().asString("now: %5.2ffps"),
						")"));
		fxPerformance.start();
	}
	
	public static void main(String[] args) {
		launch(args);
	}

}
