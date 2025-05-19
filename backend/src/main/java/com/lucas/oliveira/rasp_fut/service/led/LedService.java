package com.lucas.oliveira.rasp_fut.service.led;

import org.springframework.stereotype.Service;

import com.lucas.oliveira.rasp_fut.proxy.mqtt.MqttPublisher;

@Service
public class LedService {

  private final MqttPublisher mqttPublisher;

  public LedService(MqttPublisher mqttPublisher) {
    this.mqttPublisher = mqttPublisher;
  }

  public String turnOn() {
    return mqttPublisher.sendMessage("/led", "on");
  }

  public String turnOff() {
    return mqttPublisher.sendMessage("/led", "off");
  }

}
