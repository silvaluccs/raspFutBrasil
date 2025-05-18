package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import java.util.logging.Logger;

import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.messaging.Message;
import org.springframework.stereotype.Component;

@Component
public class MqttMessageHandler {

  private final Logger logger = Logger.getLogger(MqttMessageHandler.class.getName());

  @ServiceActivator(inputChannel = "mqttInboundChannel")
  public void handleMessage(Message<?> message) {

    String topic = (String) message.getHeaders().get("mqtt_topic");
    String payload = (String) message.getPayload();

    logger.info("Received message on topic " + topic + ": " + payload);

  }

}
