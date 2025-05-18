package com.lucas.oliveira.rasp_fut.model.match;

import com.lucas.oliveira.rasp_fut.model.team.Team;

import java.math.BigDecimal;

public class Match {

  private final Team homeTeam;
  private final Team awayTeam;

  private Integer homeScore;
  private Integer awayScore;

  private BigDecimal time;
  private String status;

  public Match(Team homeTeam, Team awayTeam, Integer homeScore, Integer awayScore, String status, BigDecimal time) {
    this.homeTeam = homeTeam;
    this.awayTeam = awayTeam;
    this.homeScore = homeScore;
    this.awayScore = awayScore;
    this.status = status;
    this.time = time;
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

}
