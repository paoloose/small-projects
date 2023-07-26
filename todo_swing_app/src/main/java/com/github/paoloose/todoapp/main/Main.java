package com.github.paoloose.todoapp.main;

import com.github.paoloose.todoapp.utils.ArgumentsParser;
import com.github.paoloose.todoapp.utils.ArgumentsParser.AppArguments;
import com.github.paoloose.todoapp.views.gui.MainWindowView;

public class Main {
    static AppArguments arguments;
    static String appTitle = "Todo App";

    // public-static-void-main-string-args ⚰️
    public static void main(String[] args) {
        arguments = ArgumentsParser.parse(args);

        switch (arguments.executionMode()) {
            case GUI -> Main.startGUIMode();
            case CLI -> Main.startCLIMode();
            default -> {
                System.out.println("Execution mode not supported");
                System.exit(1);
            }
        }
    }

    public static void startGUIMode() {
        System.out.println("starting gui model!!!");
        new MainWindowView(appTitle);
    }

    public static void startCLIMode() {
        System.out.println("starting cli model!!!");
    }
}
