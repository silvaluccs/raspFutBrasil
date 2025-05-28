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
    String topic = (String) message.getHeaders().get("mqtt_receivedTopic");

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

    Integer index = Integer.parseInt(payload);

    if (topic.equals("/dados_tempo")) {

      map.put("tempo_partida", matchDTOs.get(index).time());
      map.put("data_partida", matchDTOs.get(index).date());
      map.put("horario_partida", matchDTOs.get(index).hour());

      String json = objectMapper.writeValueAsString(map);

      mqttPublisher.sendMessage("/tempo_jogo", json);
      return;

    }

    map.put("status", matchDTOs.get(index).status());
    map.put("time_casa", matchDTOs.get(index).homeTeam());
    map.put("time_fora", matchDTOs.get(index).awayTeam());

    String placarCasa, placarFora;
    placarCasa = String.valueOf(matchDTOs.get(index).homeScore());
    placarFora = String.valueOf(matchDTOs.get(index).awayScore());

    map.put("placar_casa", placarCasa);
    map.put("placar_fora", placarFora);

    String json = objectMapper.writeValueAsString(map);

    mqttPublisher.sendMessage("/jogos", json);
  }

}
