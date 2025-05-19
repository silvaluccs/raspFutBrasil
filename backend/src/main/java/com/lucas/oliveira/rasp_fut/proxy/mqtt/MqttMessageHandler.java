package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import java.util.logging.Logger;
import java.util.Map;

import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.Message;
import org.springframework.stereotype.Component;

@Component
public class MqttMessageHandler {

  private final Logger logger = Logger.getLogger(MqttMessageHandler.class.getName());

  @ServiceActivator(inputChannel = "mqttInboundChannel")
  public void handleMessage(Message<?> message) {

    String payload = (String) message.getPayload();

    Map<String, Object> headers = message.getHeaders();

    if (headers.isEmpty()) {
      logger.info("No headers found");
    } else {
      headers.forEach((key, value) -> logger.info(key + ": " + value));

    }
    logger.info("Payload: " + payload);

  }

}
