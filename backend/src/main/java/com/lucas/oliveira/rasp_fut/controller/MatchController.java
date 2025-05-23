package com.lucas.oliveira.rasp_fut.controller;

import java.util.List;
import java.util.Optional;

import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RestController;

import com.lucas.oliveira.rasp_fut.model.match.Match;
import com.lucas.oliveira.rasp_fut.model.match.MatchDTO;
import com.lucas.oliveira.rasp_fut.service.MatchService;

@RestController
public class MatchController {

  private final MatchService matchService;

  public MatchController(MatchService matchService) {
    this.matchService = matchService;
  }

  @GetMapping("/matches")
  public List<Match> getMatches() {

    return matchService.getMatches();

  }

  @GetMapping("/matches/{matchId}")
  public ResponseEntity<MatchDTO> getMatch(@PathVariable Long matchId) {

    Optional<MatchDTO> matchDTO = matchService.getMatch(matchId);

    if (matchDTO.isEmpty()) {
      return ResponseEntity.notFound().build();
    }

    return ResponseEntity.ok(matchDTO.get());

  }

}
