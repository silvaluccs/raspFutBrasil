package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import java.util.logging.Logger;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.Message;
import org.springframework.stereotype.Component;

import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.lucas.oliveira.rasp_fut.model.match.Match;
import com.lucas.oliveira.rasp_fut.model.match.MatchDTO;
import com.lucas.oliveira.rasp_fut.service.MatchService;

@Component
public class MqttMessageHandler {

  private final Logger logger = Logger.getLogger(MqttMessageHandler.class.getName());

  private final MqttPublisher mqttPublisher;
  private final MatchService matchService;

  public MqttMessageHandler(MqttPublisher mqttPublisher, MatchService matchService) {
    this.matchService = matchService;
    this.mqttPublisher = mqttPublisher;
  }

  @ServiceActivator(inputChannel = "mqttInboundChannel")
  public void handleMessage(Message<?> message) throws Exception {

    String payload = (String) message.getPayload();

    List<Match> matches = matchService.getMatches();
    List<MatchDTO> matchDTOs = new ArrayList<>();

    ObjectMapper objectMapper = new ObjectMapper();

    for (Match match : matches) {

      String homeTeam = match.getHomeTeam().getShortName();
      String awayTeam = match.getAwayTeam().getShortName();
      String status = match.getStatus();
      String time = match.getTime().toString();
      Integer homeScore = match.getHomeScore();
      Integer awayScore = match.getAwayScore();
      String date = match.getDate();
      String hour = match.getHour();

      MatchDTO matchDTO = new MatchDTO(homeTeam, awayTeam, status, time, homeScore, awayScore, date, hour);

      matchDTOs.add(matchDTO);

    }

    Map<String, Object> map = new HashMap<>();

    map.put("status", matchDTOs.get(0).status());
    map.put("time_casa", matchDTOs.get(0).homeTeam());
    map.put("time_fora", matchDTOs.get(0).awayTeam());

    String json = objectMapper.writeValueAsString(map);

    mqttPublisher.sendMessage("/jogos", json);
  }

}
