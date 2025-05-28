package com.lucas.oliveira.rasp_fut.service;

import java.math.BigDecimal;
import java.time.OffsetDateTime;
import java.time.format.DateTimeFormatter;
import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;

import com.fasterxml.jackson.databind.JsonNode;
import com.lucas.oliveira.rasp_fut.model.match.Match;
import com.lucas.oliveira.rasp_fut.model.match.MatchDTO;
import com.lucas.oliveira.rasp_fut.model.team.Team;
import com.lucas.oliveira.rasp_fut.proxy.api.WebFutebolClient;

@Service
public class MatchService {

  private final WebFutebolClient webFutebolClient;

  public MatchService(WebFutebolClient webFutebolClient) {
    this.webFutebolClient = webFutebolClient;
  }

  public List<Match> getMatches() {

    JsonNode dataBrazilFootball = webFutebolClient.getFootballMatchs();

    JsonNode matchsNode = dataBrazilFootball.get("games");

    List<Match> matchs = new ArrayList<>();

    matchsNode.forEach(matchNode -> {

      Match match = new Match();

      Integer time = matchNode.get("gameTime").asInt();

      match.setTime(time);
      match.setStatus(matchNode.get("shortStatusText").asText());
      String date = matchNode.get("startTime").asText();
      OffsetDateTime offsetDateTime = OffsetDateTime.parse(date);

      String dateFormat = offsetDateTime.format(DateTimeFormatter.ofPattern("dd/MM/yyyy"));
      String timeFormat = offsetDateTime.format(DateTimeFormatter.ofPattern("HH:mm"));

      match.setDate(dateFormat);
      match.setHour(timeFormat);
      JsonNode homeTeamNode = matchNode.get("homeCompetitor");
      JsonNode awayTeamNode = matchNode.get("awayCompetitor");

      Team homeTeam = new Team();

      homeTeam.setId(homeTeamNode.get("id").asLong());
      homeTeam.setName(homeTeamNode.get("name").asText());
      homeTeam.setShortName(homeTeamNode.get("symbolicName").asText());

      Team awayTeam = new Team();

      awayTeam.setId(awayTeamNode.get("id").asLong());
      awayTeam.setName(awayTeamNode.get("name").asText());
      awayTeam.setShortName(awayTeamNode.get("symbolicName").asText());

      match.setHomeTeam(homeTeam);
      match.setAwayTeam(awayTeam);

      match.setHomeScore(homeTeamNode.get("score").asInt());
      match.setAwayScore(awayTeamNode.get("score").asInt());

      matchs.add(match);

    });

    return matchs;

  }

  public Optional<MatchDTO> getMatch(Long matchId) {

    JsonNode matchStats = webFutebolClient.getFootballMatch(matchId);

    if (!(matchStats.has("games"))) {
      return Optional.ofNullable(null);
    }

    JsonNode matchNode = matchStats.get("games").get(0);

    Long id = matchNode.get("id").asLong();

    String homeTeam = matchNode.get("homeCompetitor").get("symbolicName").asText();
    Integer homeScore = matchNode.get("homeCompetitor").get("score").asInt();

    String awayTeam = matchNode.get("awayCompetitor").get("symbolicName").asText();
    Integer awayScore = matchNode.get("awayCompetitor").get("score").asInt();

    String status = matchNode.get("statusText").asText();

    Integer timeInt = matchNode.get("gameTime").asInt();
    String time = String.valueOf(timeInt);

    MatchDTO matchDTO = new MatchDTO(homeTeam, awayTeam, status, time.toString(), homeScore, awayScore, "", "");

    return Optional.of(matchDTO);

  }

}
