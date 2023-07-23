package com.github.paoloose.todoapp.main;

import javax.swing.SwingUtilities;

import com.github.paoloose.todoapp.utils.ArgumentsParser;
import com.github.paoloose.todoapp.utils.ArgumentsParser.AppArguments;

public class Main {
    static AppArguments arguments;

    // public-static-void-main-string-args
    public static void main(String[] args) {
        arguments = ArgumentsParser.parse(args);

        switch (arguments.executionMode()) {
            case GUI -> startCLIMode();
            case CLI -> startCLIMode();
            default -> {
                System.out.println("Execution mode not supported");
                System.exit(1);
            }
        }
    }

    public static void startGUIMode() {
        Runnable runnable = () -> {
        };

        SwingUtilities.invokeLater(() -> {

        });
    }

    public static void startCLIMode() {

    }
}
