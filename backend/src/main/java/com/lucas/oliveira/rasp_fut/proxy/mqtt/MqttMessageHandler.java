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
import com.lucas.oliveira.rasp_fut.model.league.League;
import com.lucas.oliveira.rasp_fut.model.league.Leagues;
import com.lucas.oliveira.rasp_fut.model.match.Match;
import com.lucas.oliveira.rasp_fut.model.match.MatchDTO;
import com.lucas.oliveira.rasp_fut.service.MatchService;

@Component
public class MqttMessageHandler {

  private final Logger logger = Logger.getLogger(MqttMessageHandler.class.getName());

  private final MqttPublisher mqttPublisher;
  private final MatchService matchService;
  private League league;

  public MqttMessageHandler(MqttPublisher mqttPublisher, MatchService matchService, League league) {
    this.matchService = matchService;
    this.mqttPublisher = mqttPublisher;
    this.league = league;
  }

  @ServiceActivator(inputChannel = "mqttInboundChannel")
  public void handleMessage(Message<?> message) throws Exception {

    String payload = (String) message.getPayload();
    String topic = (String) message.getHeaders().get("mqtt_receivedTopic");

    ObjectMapper objectMapper = new ObjectMapper();

    if (payload.equalsIgnoreCase("config init request")) {

      mqttPublisher.sendMessage("/log", "[BACKEND] send size of leagues");

      String size = String.valueOf(matchService.getLeagues().size());

      mqttPublisher.sendMessage("/setup/leagues/size", size);
      return;

    }

    if (topic.equals("/setup")) {
      mqttPublisher.sendMessage("/log", "[BACKEND] send init setup for raspberry pi");

      Integer index = Integer.parseInt(payload);

      List<String> leagues = matchService.getLeagues();

      String nome = leagues.get(index);

      mqttPublisher.sendMessage("/setup/leagues", nome);
      return;

    }

    if (topic.equals("/liga")) {

      if (payload.equalsIgnoreCase("B")) {
        league.setName(Leagues.BRASILEIRA_B);
        mqttPublisher.sendMessage("/log", "[BACKEND] change league to BRASILEIRA_B");
        return;
      }

      if (payload.equalsIgnoreCase("A")) {
        league.setName(Leagues.BRASILEIRA_A);
        mqttPublisher.sendMessage("/log", "[BACKEND] change league to BRASILEIRA_A");
        return;
      }

    }

    List<Match> matches = matchService.getMatches(league.getId());
    List<MatchDTO> matchDTOs = new ArrayList<>();

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

    if (topic.equals("/tamanho_dados")) {

      Integer size = matchDTOs.size();

      mqttPublisher.sendMessage("/jogos_tamanho", String.valueOf(size));
      return;

    }

    try {
      Integer index = Integer.parseInt(payload);

      if (index > matchDTOs.size()) {
        logger.warning("[backend] O index informado é maior que o número de matches");
        mqttPublisher.sendMessage("/log", "O index informado é maior que o número de matches");
        return;
      }

      if (topic.equals("/dados_tempo")) {

        Integer tempoPartida = Integer.parseInt(matchDTOs.get(index).time());
        map.put("tempo_partida", tempoPartida);
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

    } catch (Exception e) {
      logger.warning("Erro ao converter o payload para um Integer");
      mqttPublisher.sendMessage("/log", "[backend] Erro ao converter o payload para um Integer");
      return;
    }
  }

}
