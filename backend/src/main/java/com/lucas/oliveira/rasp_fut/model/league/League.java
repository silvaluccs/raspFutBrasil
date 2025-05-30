package com.lucas.oliveira.rasp_fut.model.league;

import org.springframework.stereotype.Component;

@Component
public class League {

  private Long id;
  private Leagues name;

  public League() {

    this.id = 113L;
    this.name = Leagues.BRASILEIRA_A;

  }

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public Leagues getName() {
    return name;
  }

  public void setName(Leagues name) {
    this.name = name;
  }

}
