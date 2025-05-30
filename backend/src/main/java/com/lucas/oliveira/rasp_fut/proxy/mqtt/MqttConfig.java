package com.lucas.oliveira.rasp_fut.proxy.mqtt;

import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.integration.annotation.ServiceActivator;
import org.springframework.integration.channel.DirectChannel;
import org.springframework.integration.core.MessageProducer;
import org.springframework.integration.mqtt.core.DefaultMqttPahoClientFactory;
import org.springframework.integration.mqtt.inbound.MqttPahoMessageDrivenChannelAdapter;
import org.springframework.integration.mqtt.outbound.MqttPahoMessageHandler;
import org.springframework.integration.mqtt.support.DefaultPahoMessageConverter;
import org.springframework.messaging.MessageChannel;
import org.springframework.messaging.MessageHandler;

@Configuration
public class MqttConfig {
  private final MqttProperties mqttProperties;

  public MqttConfig(MqttProperties mqttProperties) {
    this.mqttProperties = mqttProperties;
    mqttProperties.getTopics().add("/dados_tempo");
    mqttProperties.getTopics().add("/tamanho_dados");
    mqttProperties.getTopics().add("/setup");
  }

  @Bean
  DefaultMqttPahoClientFactory mqttClientFactory() {
    MqttConnectOptions options = new MqttConnectOptions();
    options.setServerURIs(new String[] { mqttProperties.getBrokerUrl() });
    options.setUserName(mqttProperties.getUsername());
    options.setPassword(mqttProperties.getPassword().toCharArray());
    options.setAutomaticReconnect(true);
    options.setCleanSession(true);
    options.setConnectionTimeout(10);

    DefaultMqttPahoClientFactory factory = new DefaultMqttPahoClientFactory();
    factory.setConnectionOptions(options);
    return factory;
  }

  @Bean
  MessageChannel mqttOutboundChannel() {
    return new DirectChannel();
  }

  @Bean
  @ServiceActivator(inputChannel = "mqttOutboundChannel")
  MessageHandler mqttOutbound() {
    MqttPahoMessageHandler messageHandler = new MqttPahoMessageHandler(mqttProperties.getClientId(),
        mqttClientFactory());
    messageHandler.setAsync(true);
    messageHandler.setDefaultTopic("/times");
    return messageHandler;
  }

  @Bean
  MessageChannel mqttInboundChannel() {
    return new DirectChannel();
  }

  @Bean
  MessageProducer mqttInbound() {
    MqttPahoMessageDrivenChannelAdapter adapter = new MqttPahoMessageDrivenChannelAdapter(
        mqttProperties.getClientId() + "-inbound",
        mqttClientFactory(),
        mqttProperties.getTopics().toArray(new String[0]));
    adapter.setCompletionTimeout(5000);
    adapter.setConverter(new DefaultPahoMessageConverter());
    adapter.setQos(mqttProperties.getQos());
    adapter.setOutputChannel(mqttInboundChannel());
    return adapter;
  }
}
