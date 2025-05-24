package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import java.util.Optional;
import java.util.logging.Logger;

import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Component;

import com.lucas.oliveira.rasp_fut.model.match.MatchDTO;
import com.lucas.oliveira.rasp_fut.service.MatchService;

import jakarta.annotation.PostConstruct;

@Component
public class MqttMatchUpdater {

  private final Logger logger = Logger.getLogger(MqttMatchUpdater.class.getName());

  private final MqttPublisher mqttPublisher;
  private final MatchService matchService;

  private final long MATCH_ID = 4147816L;

  private boolean jogoIniciado = false;

  public MqttMatchUpdater(MqttPublisher mqttPublisher, MatchService matchService) {
    this.mqttPublisher = mqttPublisher;
    this.matchService = matchService;
  }

  @PostConstruct
  public void init() {
    logger.info("MqttMatchUpdater iniciado.");
  }

  @Scheduled(fixedRate = 1000)
  public void atualizarDadosDoJogo() {
    Optional<MatchDTO> matchDTO = matchService.getMatch(MATCH_ID);

    if (matchDTO.isPresent()) {
      MatchDTO match = matchDTO.get();

      if (!jogoIniciado) {
        logger.info("Jogo iniciado");
        mqttPublisher.sendMessage("/jogo/time_casa", match.homeTeam());
        mqttPublisher.sendMessage("/jogo/time_fora", match.awayTeam());
        jogoIniciado = true;
      }

      mqttPublisher.sendMessage("/jogo/placar_casa", match.homeScore().toString());
      mqttPublisher.sendMessage("/jogo/placar_fora", match.awayScore().toString());
      mqttPublisher.sendMessage("/jogo/tempo", match.time());
      mqttPublisher.sendMessage("/jogo/status", match.status());
    } else {
      logger.info("Esperando jogo...");
    }
  }
}
