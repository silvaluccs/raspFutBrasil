package com.lucas.oliveira.rasp_fut.exception;

public class TeamNotFoundException extends RuntimeException {

  private static final long serialVersionUID = 1L;

  public TeamNotFoundException(String message) {
    super(message);
  }

}
