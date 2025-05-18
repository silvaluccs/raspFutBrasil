package com.lucas.oliveira.rasp_fut.controller;

import java.util.List;
import java.util.Optional;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.bind.annotation.DeleteMapping;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.PostMapping;

import org.springframework.web.bind.annotation.RequestBody;

import com.lucas.oliveira.rasp_fut.service.TeamService;
import com.lucas.oliveira.rasp_fut.exception.TeamNotFoundException;
import com.lucas.oliveira.rasp_fut.model.team.Team;

@RestController
public class TeamController {

  private final TeamService teamService;

  public TeamController(TeamService teamService) {
    this.teamService = teamService;
  }

  @GetMapping("/teams")
  public ResponseEntity<List<Team>> getAllTeams() {

    List<Team> teams = this.teamService.findAll();

    return ResponseEntity.ok(teams);

  }

  @GetMapping("/teams/{id}")
  public ResponseEntity<Team> getTeam(@PathVariable Long id) {

    try {

      Optional<Team> teamOptional = this.teamService.findById(1L);

      if (teamOptional.isEmpty()) {
        return ResponseEntity.notFound().build();
      }

      return ResponseEntity.ok(teamOptional.get());

    } catch (TeamNotFoundException e) {
      return ResponseEntity.badRequest().build();
    }

  }

  @PostMapping("/teams")
  public ResponseEntity<?> createTeam(@RequestBody Team team) {

    try {
      Team teamCreated = this.teamService.save(team);

      return ResponseEntity.ok(teamCreated);
    } catch (TeamNotFoundException e) {
      return ResponseEntity.badRequest().body(e.getMessage());
    }

  }

  @DeleteMapping("/teams/{id}")
  public ResponseEntity<?> deleteTeam(@PathVariable Long id) {

    try {
      this.teamService.delete(id);
      return ResponseEntity.ok().build();
    } catch (TeamNotFoundException e) {
      return ResponseEntity.badRequest().body(e.getMessage());
    }

  }

}
