package com.lucas.oliveira.rasp_fut.model.match;

import com.lucas.oliveira.rasp_fut.model.team.Team;

import java.math.BigDecimal;

public class Match {

  private Team homeTeam;
  private Team awayTeam;

  private Integer homeScore;
  private Integer awayScore;

  private BigDecimal time;
  private String status;

  private String date;
  private String hour;

  public Match(Team homeTeam, Team awayTeam, Integer homeScore, Integer awayScore, String status, BigDecimal time,
      String date) {
    this.homeTeam = homeTeam;
    this.awayTeam = awayTeam;
    this.homeScore = homeScore;
    this.awayScore = awayScore;
    this.status = status;
    this.time = time;
    this.date = date;
  }

  public Match() {
    this.homeTeam = null;
    this.awayTeam = null;
    this.homeScore = null;
    this.awayScore = null;
    this.status = null;
    this.time = null;
    this.date = null;
  }

  public boolean isFinished() {
    return this.status.equals("Fim");
  }

  public boolean willStart() {
    return this.status.equals("Prog.");
  }

  public Team getHomeTeam() {
    return this.homeTeam;
  }

  public Team getAwayTeam() {
    return this.awayTeam;
  }

  public Integer getHomeScore() {
    return this.homeScore;
  }

  public Integer getAwayScore() {
    return this.awayScore;
  }

  public BigDecimal getTime() {
    return this.time;
  }

  public String getStatus() {
    return this.status;
  }

  public String getDate() {
    return this.date;
  }

  public void setDate(String date) {
    this.date = date;
  }

  public void setHour(String hour) {
    this.hour = hour;
  }

  public String getHour() {
    return this.hour;
  }

  public void setHomeScore(Integer homeScore) {
    this.homeScore = homeScore;
  }

  public void setAwayScore(Integer awayScore) {
    this.awayScore = awayScore;
  }

  public void setTime(BigDecimal time) {
    this.time = time;
  }

  public void setTime(Integer time) {
    this.time = BigDecimal.valueOf(time);
  }

  public void setStatus(String status) {
    this.status = status;
  }

  public void setHomeTeam(Team homeTeam) {
    this.homeTeam = homeTeam;
  }

  public void setAwayTeam(Team awayTeam) {
    this.awayTeam = awayTeam;
  }

}
