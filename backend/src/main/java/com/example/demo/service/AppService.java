package com.example.demo.service;

import com.example.demo.model.Partida;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;

import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

import java.io.IOException;
import java.util.Iterator;
import java.util.List;
import java.util.ArrayList;

@Service
public class AppService {

  private final RestTemplate restTemplate;
  private final ObjectMapper objectMapper;

  public AppService(RestTemplate restTemplate, ObjectMapper objectMapper) {
    this.restTemplate = restTemplate;
    this.objectMapper = objectMapper;
  }

  public List<Partida> getPartidas() throws IOException {

    String url = "https://webws.365scores.com/web/games/current/?appTypeId=5&langId=31&timezoneName=America/Bahia&userCountryId=21&competitions=113&showOdds=false&includeTopBettingOpportunity=1&topBookmaker=156&lastUpdateId=5187162583";

    String respostaJson = restTemplate.getForObject(url, String.class);

    JsonNode rootNode = objectMapper.readTree(respostaJson);

    JsonNode partidasNode = rootNode.path("games");

    List<Partida> partidas = new ArrayList<>();

    Iterator<JsonNode> iter = partidasNode.elements();

    int index = 0;
    while (iter.hasNext() && index < 10) {
      JsonNode partidaNode = iter.next();

      JsonNode timeCasaNode = partidaNode.path("homeCompetitor");
      String timeCasa = timeCasaNode.path("name").asText();

      JsonNode timeForaNode = partidaNode.path("awayCompetitor");
      String timeFora = timeForaNode.path("name").asText();

      partidas.add(new Partida(timeCasa, timeFora));

      index++;

    }

    return partidas;

  }

}
