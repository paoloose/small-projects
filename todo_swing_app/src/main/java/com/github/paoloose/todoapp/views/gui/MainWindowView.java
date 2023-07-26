package com.github.paoloose.todoapp.views.gui;

import javax.swing.JFrame;
import javax.swing.UIManager;

public class MainWindowView extends JFrame {

    private static MainWindowView INSTANCE = null;
    private TodoListView todoList = null;

    public MainWindowView(String title) {
        if (INSTANCE != null) {
            throw new IllegalStateException("Who instantiate me twice?");
        }
        MainWindowView.INSTANCE = this;

        try {
            String howDoILook = UIManager.getSystemLookAndFeelClassName();
            UIManager.setLookAndFeel(howDoILook);
        }
        catch(Exception e) {
            e.printStackTrace();
        }

        this.setTitle(title);
        this.setSize(400, 400);
        this.setResizable(true);
        this.setDefaultCloseOperation(EXIT_ON_CLOSE);
        this.setLayout(null);

        this.todoList = new TodoListView();
        this.todoList.setVisible(true);
        this.todoList.setSize(100, 100);
        this.todoList.setBackground(java.awt.Color.BLACK);
        this.add(this.todoList);

        this.setVisible(true);
    }
}
