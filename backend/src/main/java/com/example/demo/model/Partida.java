package com.example.demo.model;

public class Partida {

  private String timeCasa;

  private String timeFora;

  public Partida(String timeCasa, String timeFora) {
    this.timeCasa = timeCasa;
    this.timeFora = timeFora;
  }

  public String getTimeCasa() {
    return timeCasa;
  }

  public void setTimeCasa(String timeCasa) {
    this.timeCasa = timeCasa;
  }

  public String getTimeFora() {
    return timeFora;
  }

  public void setTimeFora(String timeFora) {
    this.timeFora = timeFora;
  }

  @Override
  public String toString() {

    return timeCasa + " X " + timeFora + "\n";

  }

}
