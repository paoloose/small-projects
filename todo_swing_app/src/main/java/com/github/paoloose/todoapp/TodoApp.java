package com.github.paoloose.todoapp;
import javax.swing.UIManager;

public class TodoApp {

    private static TodoApp INSTANCE = null;

    TodoApp() {
        if (INSTANCE != null) {
            throw new IllegalStateException("Who instantiate me twice?");
        }
        INSTANCE = this;

        try {
            String howDoILook = UIManager.getSystemLookAndFeelClassName();
            UIManager.setLookAndFeel(howDoILook);
            System.out.println("Look and feel: " + howDoILook);
        }
        catch(Exception e) {
            e.printStackTrace();
        }
    }

    // repeat after me: public static final void main string args
    public static void main(String[] args) {
        new TodoApp();
    }
}
