package com.github.paoloose.todoapp.models.todo;

import java.io.Serializable;

public class TodoItem implements Serializable {
    private String title;
    private String details;
    private boolean done;

    public TodoItem() {
        this.title = "Untiteld TODO";
        this.details = "";
        this.done = false;
    }

    public TodoItem(String title) {
        this.title = title;
        this.details = "";
        this.done = false;
    }

    public String getTitle() {
        return this.title;
    }

    public String getDetails() {
        return this.details;
    }

    public boolean isDone() {
        return this.done;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public void setDetails(String details) {
        this.details = details;
    }

    public void setDone(boolean done) {
        this.done = done;
    }

    // TODO: shoulnt this be in the controller?
    public String toString() {
        String done = this.done ? "[X]" : "[ ]";
        return done + " " + this.title + "\n" + this.details;
    }
}
