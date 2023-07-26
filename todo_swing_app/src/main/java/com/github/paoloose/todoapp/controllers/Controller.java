package com.github.paoloose.todoapp.controllers;

import com.github.paoloose.todoapp.views.View;

public abstract class Controller {
    private View view;

    void setView(View view) {
        this.view = view;
    }
}
