export class MatchNotFoundException extends Error {
  constructor(message: string = "A partida não foi encotrada.") {
    super(message);
    this.name = "MatchNotFoundException";
  }
}

export class MqttConnectionException extends Error {
  constructor(message: string = "Falha ao conectar ao MQTT.") {
    super(message);
    this.name = "MqttConnectionException";
  }
}

export class MqttSubscriptionException extends Error {
  constructor(message: string = "Falha ao subscrito ao tópico MQTT.") {
    super(message);
    this.name = "MqttSubscriptionException";
  }
}
