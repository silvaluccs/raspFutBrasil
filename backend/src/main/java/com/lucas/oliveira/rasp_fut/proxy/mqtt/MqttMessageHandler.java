package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import java.util.logging.Logger;
import java.util.Map;
import java.util.Optional;

import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.Message;
import org.springframework.stereotype.Component;

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
  public void handleMessage(Message<?> message) {

    String payload = (String) message.getPayload();

    logger.info("payload: " + payload);

    if (payload.contains("esperando")) {
      logger.info("esperando jogo");
      Optional<MatchDTO> matchDTO = matchService.getMatch(4147816L);

      if (matchDTO.isPresent()) {

        mqttPublisher.sendMessage("/jogo/time_casa", matchDTO.get().homeTeam());
        mqttPublisher.sendMessage("/jogo/time_fora", matchDTO.get().awayTeam());
        mqttPublisher.sendMessage("/jogo/placar_casa", matchDTO.get().homeScore().toString());
        mqttPublisher.sendMessage("/jogo/placar_fora", matchDTO.get().awayScore().toString());
        mqttPublisher.sendMessage("/jogo/tempo", matchDTO.get().time());

        mqttPublisher.sendMessage("/jogo/status", matchDTO.get().status());

        logger.info("jogo iniciado");

      }

    }

  }

}
