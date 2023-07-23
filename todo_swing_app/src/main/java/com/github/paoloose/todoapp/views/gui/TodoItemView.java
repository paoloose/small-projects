package com.github.paoloose.todoapp.views.gui;
import javax.swing.JFrame;
import javax.swing.UIManager;
import javax.swing.WindowConstants;

public class TodoItemView {
public class TodoApp {

    private static TodoApp INSTANCE = null;
    private JFrame window = null;

    public TodoApp(String title) {
        if (INSTANCE != null) {
            throw new IllegalStateException("Who instantiate me twice?");
        }
        INSTANCE = this;

        try {
            String howDoILook = UIManager.getSystemLookAndFeelClassName();
            UIManager.setLookAndFeel(howDoILook);
        }
        catch(Exception e) {
            e.printStackTrace();
        }

        // initialize window
        this.window = new JFrame(title);

        this.window.setSize(400, 400);
        this.window.setDefaultCloseOperation(WindowConstants.EXIT_ON_CLOSE);
        this.window.setVisible(true);
    }
}

}
