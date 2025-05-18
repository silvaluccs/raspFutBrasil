package com.lucas.oliveira.rasp_fut.model.team;

import jakarta.persistence.Entity;
import jakarta.persistence.Id;
import jakarta.persistence.JoinColumn;
import jakarta.persistence.Table;

@Entity
@Table(name = "teams")
/**
 * Teams are the teams that participate in the match.
 * The teams are identified by their short name.
 * The short name is the name of the team in the competition.
 * 
 * @author Lucas Oliveira
 */
public class Team {

  @Id
  private Long id;

  @JoinColumn(nullable = false)
  private String name;

  @JoinColumn(nullable = false)
  private String shortName;

  public Long getId() {
    return id;
  }

  public void setId(Long id) {
    this.id = id;
  }

  public String getName() {
    return name;
  }

  public void setName(String name) {
    this.name = name;
  }

  public String getShortName() {
    return shortName;
  }

  public void setShortName(String shortName) {
    this.shortName = shortName;
  }

  @Override
  public String toString() {

    return "Team{" +
        "id=" + id +
        ", name='" + name + '\'' +
        ", shortName='" + shortName + '\'' +
        '}';

  }

}
