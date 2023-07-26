package com.github.paoloose.todoapp.utils;

import java.util.Objects;

public class ArgumentsParser {
    public static enum ExecutionMode {
        GUI,
        CLI
    }
    public record AppArguments(
        ExecutionMode executionMode) {

        public AppArguments {
            Objects.requireNonNull(executionMode);
        }
    }

    public static AppArguments parse(String[] args) {
        ExecutionMode mode = ExecutionMode.GUI;

        for (String arg : args) {
            if (arg.equals("--cli")) {
                mode = ExecutionMode.CLI;
            }
        }

        return new AppArguments(mode);
    }
}
