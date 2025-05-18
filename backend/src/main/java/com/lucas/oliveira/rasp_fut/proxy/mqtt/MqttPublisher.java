package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.messaging.MessageChannel;
import org.springframework.stereotype.Component;

import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;
import java.util.logging.Logger;

import org.springframework.messaging.MessageHeaders;
import org.springframework.messaging.support.MessageBuilder;

@Component
public class MqttPublisher {
  private final MessageChannel mqttOutboundChannel;
  private final Logger logger = Logger.getLogger(MqttPublisher.class.getName());

  public MqttPublisher(@Qualifier("mqttOutboundChannel") MessageChannel mqttOutboundChannel) {
    this.mqttOutboundChannel = mqttOutboundChannel;
  }

  public String sendMessage(String topic, String message) {
    Map<String, Object> headers = new HashMap<>();
    headers.put("mqtt_topic", topic);

    mqttOutboundChannel.send(
        MessageBuilder.createMessage(
            message.getBytes(StandardCharsets.UTF_8),
            new MessageHeaders(headers)));

    String logMessage = "Message sent to [" + topic + "]: " + message;
    logger.info(logMessage);
    return logMessage;
  }
}
